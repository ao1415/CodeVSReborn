#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"

struct Data {

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

						top.chain += top.info.field.dropPack(packs[turn], com);
						top.info.score += top.chain.score;
						top.info.diffScore += top.chain.score;
						top.info.garbage -= top.chain.garbage;
						if (top.chain.chain > 0)
							top.info.gauge += GaugeAdd;

						if (top.info.garbage >= Witdh)
						{
							top.info.field.dropGarbage();
							top.info.garbage -= Witdh;
						}

						next.push(std::move(top));
					}
				}

				if (qData.top().info.gauge >= SkillCost)
				{
					auto top = qData.top();

					Command com(true);

					top.chain += top.info.field.useSkill();
					top.info.score += top.chain.score;
					top.info.diffScore += top.chain.score;
					top.info.garbage -= top.chain.garbage;
					top.info.gauge = 0;

					if (top.info.garbage >= Witdh)
					{
						top.info.field.dropGarbage();
						top.info.garbage -= Witdh;
					}

					next.push(std::move(top));
				}
				qData.pop();
			}

			qData.swap(next);
		}

		return qData.top();
	}

public:

	Ai() {

		const auto share = Share::Get();

		packs = share->packs();

	}

	std::string think() {

		const auto& share = *Share::Get();
		const auto& turn = share.turn();

		const auto& my = share.my();
		const auto& enemy = share.enemy();

		const auto& field = my.field;

		const auto enemyData = enemyThink();

		enemyData.chain.debug();
		enemyData.info.debug();

		Command com;
		com.pos = (turn % 4) * 2;
		com.rotate = 0;

		return com.toString();
	}

};
