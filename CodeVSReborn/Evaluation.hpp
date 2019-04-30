#pragma once

#include "Base.hpp"
#include "Field.hpp"
#include "Share.hpp"

namespace Config {


	/// <summary>’T¸ƒ^[ƒ“”</summary>
	constexpr int Turn = 10;
	/// <summary>’T¸‚Ìƒr[ƒ€•</summary>
	constexpr int Chokudai = 3;
	/// <summary>’T¸ŠÔ</summary>
	constexpr int ThinkTime = 1500;

	constexpr int UselessChain = 1;
	constexpr int ChainIgnition = 8;
	constexpr int SkillIgnitionScore = 300;

	/// <summary>“G‚Ì’T¸ƒ^[ƒ“”</summary>
	constexpr int EnemyThinkTurn = 3;

}

class Evaluation {
private:

	double score = 0;

	Chain maxChain(const PlayerInfo& info) {

		const auto& packs = Share::Get()->packs();

		Chain max;
		double chainScore = 0.0;

		for (int num = 1; num <= 9; num++)
		{
			auto next = info.copy();
			auto chain = next.field.dropCell(num);

			if (max.score < chain.score)
			{
				max = chain;
				chainScore = score;
			}
		}

		return max;
	}

public:

	Evaluation() {}
	Evaluation(const PlayerInfo& info, const Chain& chain, const Evaluation& prev, const int turn) {

		auto random = Random::Get();

		const auto potentialChain = maxChain(info);
		const auto skillChain = info.copy().field.useSkill();

		//score = prev.score;
		score -= chain.score;

		score += potentialChain.chain * 100;
		score += skillChain.score * 10;
		score += info.gauge / 10;

		score += random->swing();
	}

	bool operator<(const Evaluation& other) const {

		return score < other.score;
	}

	void debug() const {

		std::cerr << "Evaluation" << std::endl;
		std::cerr << "Evaluation:" << score << std::endl;

	}

};
