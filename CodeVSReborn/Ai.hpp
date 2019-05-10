#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"
#include "Evaluation.hpp"

#include "Pool.hpp"

struct Data {

	PlayerInfo info;
	std::array<Command, Config::Turn> com;

	Evaluation eval;
	Chain chain;

	bool operator<(const Data& o) const {
		return eval < o.eval;
	}

	Data() {}
	/*
	Data(const Data& other) {
		this->info = other.info;
		this->com = other.com;
		this->eval = other.eval;
		this->chain = other.chain;
	}
	*/
};

struct DataLess {
	bool operator()(const Data* x, const Data* y) const {
		return (*x) < (*y);
	}
};

struct EnemyData {
	Chain chain;
	PlayerInfo info;

	bool operator<(const EnemyData& o) const {
		return info.score < o.info.score;
	}

};

class Ai {
private:

	std::array<Pack, MaxTurn> packs;
	std::array<Command, Config::Turn> prevCom;
	int prevGarbage;

	std::unordered_set<HashBit> hashSet;

	Action decisionAction;

#ifdef _MSC_VER
	MemoryPool<Data, 2000000> pool;
#else
	MemoryPool<Data, 1500000> pool;
#endif

	[[nodiscard]]
	EnemyData enemyThink(const int garbage = 0) {

		const auto& share = *Share::Get();

		const auto& turn = share.turn();
		const auto& enemy = share.enemy();

		std::priority_queue<EnemyData> qData;

		{
			EnemyData now;
			now.info = enemy.copy();
			qData.push(now);
		}

		//1ターン目(現在のターン)
		{
			decltype(qData) next;

			while (!qData.empty())
			{
				for (int pos = 0; pos < PackDropRange; pos++)
				{
					for (int rot = 0; rot < 4; rot++)
					{
						auto top = qData.top();

						Command com(pos, rot);

						top.chain += top.info.simulation(com, packs[turn + 0]);

						if (top.info.field.isSurvival())
							next.push(std::move(top));
					}
				}

				if (qData.top().info.gauge >= SkillCost)
				{
					auto top = qData.top();

					Command com(true);

					top.chain += top.info.simulation(com, packs[turn + 0]);

					top.info.garbage += garbage;

					if (top.info.field.isSurvival())
						next.push(std::move(top));
				}
				qData.pop();
			}

			qData.swap(next);
		}

		//2ターン目移行(次移行のターン)
		for (int t = 1; t < Config::EnemyThinkTurn; t++)
		{
			if (turn + t >= MaxTurn) break;

			decltype(qData) next;

			while (!qData.empty())
			{
				for (int pos = 0; pos < PackDropRange; pos++)
				{
					for (int rot = 0; rot < 4; rot++)
					{
						auto top = qData.top();

						Command com(pos, rot);

						top.chain += top.info.simulation(com, packs[turn + t]);

						if (top.info.field.isSurvival())
							next.push(std::move(top));
					}
				}

				if (qData.top().info.gauge >= SkillCost)
				{
					auto top = qData.top();

					Command com(true);

					top.chain += top.info.simulation(com, packs[turn + t]);

					if (top.info.field.isSurvival())
						next.push(std::move(top));
				}
				qData.pop();
			}

			qData.swap(next);
		}

		if (!qData.empty())
		{
			return qData.top();
		}

		return EnemyData();
	}

	[[nodiscard]]
	std::array<std::priority_queue<Data*, std::vector<Data*>, DataLess>, static_cast<size_t>(Action::Size)> attackThink(const EnemyData& enemy) {

		const auto& share = *Share::Get();

		const auto& turn = share.turn();
		const auto& my = share.my();

		//true：攻撃判定
		const auto rule = [&](const Chain& chain) {

			if (chain.chain >= Config::ChainIgnition)
			{
				return Action::Attack;
			}

			if (chain.chain >= Config::JammingChain)
			{
				if (enemy.chain.garbage >= Config::JammingGarbage && enemy.chain.chain <= Config::ApprovalJammingChain)
				{
					return Action::Jamming;
				}
			}

			if (my.garbage >= Config::LethalGarbage)
			{
				if (my.garbage - chain.garbage <= Config::ToleranceGarbage)
				{
					return Action::Attack;
				}
			}

			return Action::Preparation;
		};

		Data now;
		now.info = my.copy();

		std::array<std::priority_queue<Data*, std::vector<Data*>, DataLess>, static_cast<size_t>(Action::Size)> actionData;

		for (int pos = 0; pos < PackDropRange; pos++)
		{
			for (int rot = 0; rot < 4; rot++)
			{
				auto next = new(pool.getAddress()) Data(now);
				//auto next = new Data(now);

				next->com[0] = Command(pos, rot);

				const auto chain = next->info.simulation(next->com[0], packs[turn]);
				next->chain = chain;

				if (next->info.field.isSurvival())
				{
					const auto hashCode = next->info.field.hash();
					if (hashSet.find(hashCode) == hashSet.cend())
					{
						hashSet.insert(hashCode);
						next->eval = Evaluation(next->info, chain, Evaluation(), turn);

						const auto action = rule(chain);

						switch (action)
						{
						case Action::Attack:
							actionData[static_cast<size_t>(Action::Attack)].push(next);
							break;
						case Action::Jamming:
							actionData[static_cast<size_t>(Action::Jamming)].push(next);
							break;
						case Action::Preparation:
							if (chain.chain <= Config::UselessChain)
								actionData[static_cast<size_t>(Action::Preparation)].push(next);
							break;
						default:
							break;
						}
					}
					else
					{
						pool.popAddress();
					}
				}
			}
		}

		if (now.info.gauge >= SkillCost)
		{
			auto next = new(pool.getAddress()) Data(now);
			//auto next = new Data(now);

			next->com[0] = Command(true);

			const auto chain = next->info.simulation(next->com[0], packs[turn]);
			next->chain = chain;

			if (next->info.field.isSurvival())
			{
				next->eval = Evaluation(next->info, chain, Evaluation(), turn);
				if (chain.score >= Config::SkillIgnitionScore)
				{
					actionData[static_cast<size_t>(Action::Attack)].push(next);
				}
				else
				{
					actionData[static_cast<size_t>(Action::Preparation)].push(next);
				}
			}
			else
			{
				pool.popAddress();
			}
		}

		return actionData;
	}

	std::priority_queue<Data*, std::vector<Data*>, DataLess> attackThink() {

		const auto enemyData1 = enemyThink();

		auto action = attackThink(enemyData1);

		if (!action[static_cast<size_t>(Action::Attack)].empty())
		{
			const auto& top = action[static_cast<size_t>(Action::Attack)].top();
			const auto garbage = top->chain.garbage;
			const auto enemyData2 = enemyThink(garbage);

			if (enemyData2.info.garbage >= enemyData1.info.garbage)
			{
				decisionAction = Action::Attack;
				return action[static_cast<size_t>(Action::Attack)];
			}
		}


		if (!action[static_cast<size_t>(Action::Jamming)].empty())
		{
			decisionAction = Action::Jamming;
			return action[static_cast<size_t>(Action::Jamming)];
		}

		decisionAction = Action::Preparation;
		return action[static_cast<size_t>(Action::Preparation)];
	}

public:

	Ai() {

		const auto share = Share::Get();

		packs = share->packs();

	}

	const Command think() {

		const auto& share = *Share::Get();
		const auto& baseTurn = share.turn();

		const auto& my = share.my();
		const auto& enemy = share.enemy();

		const auto& baseTime = my.time;
		const auto& field = my.field;

		hashSet.clear();
		pool.clear();

		std::array <std::priority_queue<Data*, std::vector<Data*>, DataLess>, Config::Turn + 1 > qData;

		auto attack = attackThink();
		qData[1].swap(attack);

		//前回の最善手をセット
		if (decisionAction == Action::Preparation && my.garbage < Width)
		{
			for (int t = 1; t < Config::Turn - 1; t++)
			{
				const int turn = baseTurn + t;
				if (turn >= MaxTurn) break;
				if (qData[t].empty()) break;

				const auto top = qData[t].top();
				{
					auto next = new(pool.getAddress()) Data(*top);
					//auto next = new Data(*top);

					next->com[t] = prevCom[t + 1];
					const auto chain = next->info.simulation(next->com[t], packs[turn]);

					if (next->info.field.isSurvival())
					{
						next->eval = Evaluation(next->info, chain, top->eval, turn);
						qData[t + 1].push(std::move(next));
					}
				}
			}
		}

		const int thinkTime = [&]() {
			if (baseTime >= 180 * 1000) return static_cast<int>(Config::ThinkTime * 8);
			if (baseTime >= 120 * 1000) return static_cast<int>(Config::ThinkTime * 2);
			if (baseTime >= 60 * 1000) return static_cast<int>(Config::ThinkTime);
			if (baseTime >= 30 * 1000) return static_cast<int>(Config::ThinkTime / 2);
			return 100;
		}();

		MilliSecTimer timer;
		timer.set(std::chrono::milliseconds(thinkTime));

		long long int loop = 0;
		long long int copy = 0;

		timer.start();

		while (!timer)
		{
			for (int t = 0; t < Config::Turn; t++)
			{
				const int turn = baseTurn + t;

				if (turn >= MaxTurn) break;

				for (int i = 0; i < Config::Chokudai; i++)
				{
					if (qData[t].empty()) break;

					const auto top = qData[t].top();

					for (int pos = 0; pos < PackDropRange; pos++)
					{
						for (int rot = 0; rot < 4; rot++)
						{
							auto next = new(pool.getAddress()) Data(*top);
							//auto next = new Data(*top);
							copy++;

							next->com[t] = Command(pos, rot);

							const auto chain = next->info.simulation(next->com[t], packs[turn]);

							if (chain.chain <= Config::UselessChain)
							{
								if (next->info.field.isSurvival())
								{
									const auto hashCode = next->info.field.hash();
									if (hashSet.find(hashCode) == hashSet.cend())
									{
										hashSet.insert(hashCode);
										next->eval = Evaluation(next->info, chain, top->eval, turn);

										qData[t + 1].push(std::move(next));
									}
									else
									{
										pool.popAddress();
									}
								}
								else
								{
									pool.popAddress();
								}
							}
							else
							{
								pool.popAddress();
							}
						}
					}

					qData[t].pop();
				}
			}
			loop++;
		}


		for (int i = Config::Turn - 1; i >= 0; i--)
		{
			if (!qData[i].empty())
			{
				const auto top = qData[i].top();
				prevCom = top->com;

				top->info.debug("My Best");
				top->eval.debug();

				std::cerr << "loop:" << loop << ", copy:" << copy << std::endl;

				switch (decisionAction)
				{
				case Action::Attack:
					std::cerr << ">> Attack" << std::endl;
					std::cerr << "chain:" << top->chain.chain << std::endl;
					break;
				case Action::Jamming:
					std::cerr << ">> Jamming" << std::endl;
					std::cerr << "chain:" << top->chain.chain << std::endl;
					break;
				case Action::Preparation:
					std::cerr << ">> Preparation" << std::endl;
					break;
				default:
					break;
				}
				std::cerr << "Memory Use Rate:" << pool.useRate() << std::endl;

				return top->com[0];
			}
		}

		std::cerr << ":'(" << std::endl;

		return Command();
	}

};
