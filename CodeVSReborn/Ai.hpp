#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"

struct Data {

};

class Ai {
private:

	std::array<Pack, MaxTurn> packs;

	std::priority_queue<Data> attack;

	void attackThink() {

		attack = std::priority_queue<Data>();

	}

	Chain enemyThink() {

		const auto& share = *Share::Get();

		const auto& turn = share.turn();
		const auto& enemy = share.enemy();

		for (int pos = 0; pos < PackDropRange; pos++)
		{
			for (int rot = 0; rot < 4; rot++)
			{
				auto field = enemy.field.copy();
				Command com;
				com.pos = pos;
				com.rotate = rot;

				const auto chain = field.dropPack(packs[turn], com);

			}
		}

		return Chain();
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

		if (my.gauge >= SkillCost)
		{
			return Command(true).toString();
		}

		std::vector<int> drop({ 0,2,4 });

		const auto table = field.getFieldArray();
		const int deadline = DangerLine + 5;

		for (int i = 0; i < (int)drop.size(); i++)
		{
			if (table[deadline][drop[i]] == Empty && table[deadline][drop[i] + 1] == Empty)
			{
				return Command(drop[i], 0).toString();
			}
		}

		int maxScore = -1;
		Command maxCom;
		for (int pos = 6; pos < PackDropRange; pos++)
		{
			for (int rot = 0; rot < 4; rot++)
			{
				Command com(pos, rot);

				auto next = field.copy();
				auto chain = next.dropPack(packs[turn], com);

				if (chain.score > maxScore)
				{
					maxScore = chain.score;
					maxCom = com;
				}
			}
		}

		return maxCom.toString();
	}

};
