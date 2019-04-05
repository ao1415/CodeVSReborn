#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"

class Ai {
private:

	std::array<Pack, MaxTurn> packs;

public:

	Ai() {

		const auto share = Share::Get();

		packs = share->packs();

	}

	std::string think() {

		const auto share = *Share::Get();
		const auto turn = share.turn();

		const auto my = share.my();
		const auto& field = my.field;

		auto next = field.copy();

		Command com;
		Chain result;
		if (my.gauge >= SkillCost)
		{
			com.skill = true;

			result = next.useSkill();
		}
		else
		{
			com.pos = rand() % PackDropRange;
			com.rotate = rand() % 4;

			result = next.dropPack(packs[turn], com);
		}


		std::cerr << "c:" << result.chain << ", s:" << result.score << std::endl;

		return com.toString();
	}

};
