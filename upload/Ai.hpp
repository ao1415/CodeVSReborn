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
	int ignition = Config::Ignition;
	std::array<Command, Config::Turn> prevCom;

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

public:

	Ai() {

		const auto share = Share::Get();

		packs = share->packs();

	}

	std::string think() {

		const auto& share = *Share::Get();
		const auto& baseTurn = share.turn();

		const auto& my = share.my();
		const auto& enemy = share.enemy();

		const auto& field = my.field;

		std::array<std::priority_queue<Data>, Config::Turn + 1> qData;

		{
			Data now;
			now.info = my.copy();

			qData[0].push(std::move(now));

			//前回の最善手をセット
			for (int t = 0; t < Config::Turn - 1; t++)
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
						int ign = top.eval.getIgnition();

						if (turn > ign)
							ign += Config::Ignition;

						next.eval = Evaluation(next.info, chain, top.eval, turn, ign);

						qData[t + 1].push(std::move(next));
					}
				}
			}
		}

		const auto enemyData = enemyThink();

		//enemyData.chain.debug();
		enemyData.info.debug();
		//enemyData.info.field.debug();

		Timer timer;
		timer.set(std::chrono::milliseconds(Config::ThinkTime));

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

								int ign = top.eval.getIgnition();

								if (turn > ign)
									ign += Config::Ignition;

								next.eval = Evaluation(next.info, chain, top.eval, turn, ign);

								qData[t + 1].push(std::move(next));
							}
						}
					}

					if (top.info.gauge >= SkillCost)
					{
						auto next = top;

						next.com[t] = Command(true);

						const auto chain = next.info.simulation(next.com[t], packs[turn]);

						if (top.info.field.isSurvival())
						{
							int ign = top.eval.getIgnition();

							if (turn > ign)
								ign += Config::Ignition;

							next.eval = Evaluation(next.info, chain, top.eval, turn, ign);

							qData[t + 1].push(std::move(next));
						}
					}

					qData[t].pop();
				}
			}
			loop++;
		}

		std::cerr << "loop:" << loop << std::endl;

		for (int i = Config::Turn - 1; i >= 0; i--)
		{
			if (!qData[i].empty())
			{
				const auto& top = qData[i].top();
				prevCom = top.com;

				top.info.debug("My Best");
				top.eval.debug();

				return top.com[0].toString();
			}
		}

		std::cerr << "むりぽ(´・ω・｀)" << std::endl;

		return Command().toString();
	}

};
