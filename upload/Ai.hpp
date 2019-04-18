#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"

struct Data {

	const static int Turn = 10;
	const static int Chokudai = 3;

	PlayerInfo info;
	std::array<Command, Turn> com;

	bool operator<(const Data& o) const {
		return info.score < o.info.score;
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

	std::priority_queue<Data> attack;

	void attackThink() {

		attack = std::priority_queue<Data>();

	}

	Chain simulation(PlayerInfo& info, const Command& com, const int turn) const {

		if (com.skill)
		{
			const auto chain = info.field.useSkill();
			info.score += chain.score;
			info.diffScore += chain.score;
			info.garbage -= chain.garbage;
			info.gauge = 0;

			if (info.garbage >= Witdh)
			{
				info.field.dropGarbage();
				info.garbage -= Witdh;
			}

			return chain;
		}
		else
		{
			const auto chain = info.field.dropPack(packs[turn], com);
			info.score += chain.score;
			info.diffScore += chain.score;
			info.garbage -= chain.garbage;

			if (chain.chain > 0)
			{
				info.gauge += GaugeAdd;
			}

			if (info.garbage >= Witdh)
			{
				info.field.dropGarbage();
				info.garbage -= Witdh;
			}

			return chain;
		}

	}

	[[nodiscard]]
	EnemyData enemyThink() {

		const auto& share = *Share::Get();

		const auto& turn = share.turn();
		const auto& enemy = share.enemy();

		std::priority_queue<EnemyData> qData;

		{
			EnemyData now;
			now.info = enemy;
			qData.push(now);
		}

		for (int t = 0; t < 2; t++)
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

						top.chain += simulation(top.info, com, turn + t);

						if (top.info.field.isSurvival())
							next.push(std::move(top));
					}
				}

				if (qData.top().info.gauge >= SkillCost)
				{
					auto top = qData.top();

					Command com(true);

					top.chain += simulation(top.info, com, turn + t);

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

		const auto enemyData = enemyThink();

		//enemyData.chain.debug();
		enemyData.info.debug();
		//enemyData.info.field.debug();

		std::array<std::priority_queue<Data>, Data::Turn + 1> qData;

		{
			Data now;
			now.info = my;

			qData[0].push(std::move(now));
		}

		Timer timer;
		timer.set(std::chrono::milliseconds(1200));

		long long int loop = 0;

		timer.start();
		while (!timer)
		{
			for (int t = 0; t < Data::Turn; t++)
			{
				const int turn = baseTurn + t;

				if (turn >= MaxTurn) break;

				for (int i = 0; i < Data::Chokudai; i++)
				{
					if (qData[t].empty()) break;

					const auto& top = qData[t].top();

					for (int pos = 0; pos < PackDropRange; pos++)
					{
						for (int rot = 0; rot < 4; rot++)
						{
							auto next = top;

							next.com[t] = Command(pos, rot);

							const auto chain = simulation(next.info, next.com[t], turn + t);

							if (next.info.field.isSurvival())
								qData[t + 1].push(std::move(next));
						}
					}

					if (top.info.gauge >= SkillCost)
					{
						auto next = top;

						next.com[t] = Command(true);

						const auto chain = simulation(next.info, next.com[t], turn + t);

						if (next.info.field.isSurvival())
							qData[t + 1].push(std::move(next));
					}

					qData[t].pop();
				}
			}
			loop++;
		}

		std::cerr << "loop:" << loop << std::endl;

		for (int i = Data::Turn - 1; i >= 0; i--)
		{
			if (!qData[i].empty())
			{
				const auto& top = qData[i].top();

				return top.com[0].toString();
			}
		}

		std::cerr << "‚Þ‚è‚Û(LEƒÖEM)" << std::endl;

		return Command().toString();
	}

};
