#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"
#include "Evaluation.hpp"

struct Data {

	PlayerInfo info;
	std::array<Command, Config::Turn> com;

	Evaluation eval;
	Chain chain;

	bool operator<(const Data& o) const {
		return eval < o.eval;
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
	bool attackFlag = false;

	std::unordered_set<HashBit> hashSet;

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
	std::priority_queue<Data> attackThink(const EnemyData& enemy) {

		const auto& share = *Share::Get();

		const auto& turn = share.turn();
		const auto& my = share.my();

		Data now;
		now.info = my.copy();

		std::priority_queue<Data> attack;
		std::priority_queue<Data> collect;

		for (int pos = 0; pos < PackDropRange; pos++)
		{
			for (int rot = 0; rot < 4; rot++)
			{
				auto next = now;

				next.com[0] = Command(pos, rot);

				const auto chain = next.info.simulation(next.com[0], packs[turn]);

				if (next.info.field.isSurvival())
				{
					const auto hashCode = next.info.field.hash();
					if (hashSet.find(hashCode) == hashSet.cend())
					{
						hashSet.insert(hashCode);
						if (chain.chain >= Config::ChainIgnition)
						{
							next.eval = Evaluation(next.info, chain, Evaluation(), turn);
							next.chain = chain;
							attack.push(std::move(next));
						}
						else if (chain.chain <= Config::UselessChain)
						{
							next.eval = Evaluation(next.info, chain, Evaluation(), turn);
							collect.push(std::move(next));
						}
					}
				}
			}
		}

		if (now.info.gauge >= SkillCost)
		{
			auto next = now;

			next.com[0] = Command(true);

			const auto chain = next.info.simulation(next.com[0], packs[turn]);

			if (next.info.field.isSurvival())
			{
				if (chain.score >= Config::SkillIgnitionScore)
				{
					next.eval = Evaluation(next.info, chain, Evaluation(), turn);
					next.chain = chain;
					attack.push(std::move(next));
				}
			}
		}

		attackFlag = false;
		if (!attack.empty())
		{
			attackFlag = true;
			return attack;
		}

		return collect;
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

		std::array<std::priority_queue<Data>, Config::Turn + 1> qData;

		const auto enemyData = enemyThink();

		//enemyData.chain.debug();
		//enemyData.info.debug();
		//enemyData.info.field.debug();

		auto attack = attackThink(enemyData);
		qData[1].swap(attack);

		//前回の最善手をセット
		/*
		if (!attackFlag)
		{
			for (int t = 1; t < Config::Turn - 1; t++)
			{
				const int turn = baseTurn + t;

				if (turn >= MaxTurn) break;

				if (qData[t].empty()) break;

				const auto& top = qData[t].top();

				{
					auto next = top;

					next.com[t] = prevCom[t + 1];

					const auto chain = next.info.simulation(next.com[t], packs[turn]);

					if (next.info.field.isSurvival())
					{
						next.eval = Evaluation(next.info, chain, top.eval, turn);

						qData[t + 1].push(std::move(next));
					}
				}
			}
		}
		*/
		const int thinkTime = [&]() {
			if (baseTime > 120 * 1000) return Config::ThinkTime * 2;
			if (baseTime > 60 * 1000) return Config::ThinkTime;
			if (baseTime > 30 * 1000) return Config::ThinkTime / 2;
			return 100;
		}();

		Timer timer;
		timer.set(std::chrono::milliseconds(thinkTime));

		long long int loop = 0;

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

					const auto& top = qData[t].top();

					for (int pos = 0; pos < PackDropRange; pos++)
					{
						for (int rot = 0; rot < 4; rot++)
						{
							auto next = top;

							next.com[t] = Command(pos, rot);

							const auto chain = next.info.simulation(next.com[t], packs[turn]);

							if (next.info.field.isSurvival())
							{
								const auto hashCode = next.info.field.hash();
								if (hashSet.find(hashCode) == hashSet.cend())
								{
									hashSet.insert(hashCode);
									next.eval = Evaluation(next.info, chain, top.eval, turn);

									qData[t + 1].push(std::move(next));
								}
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
				const auto& top = qData[i].top();
				prevCom = top.com;

				top.info.debug("My Best");
				top.eval.debug();

				std::cerr << "loop:" << loop << std::endl;
				if (attackFlag)
				{
					std::cerr << "!Ignition!" << std::endl;
					top.chain.debug();
				}
				return top.com[0];
			}
		}

		std::cerr << ":'(" << std::endl;

		return Command();
	}

};
