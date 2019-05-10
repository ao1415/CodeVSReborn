#pragma once

#include "Base.hpp"
#include "Field.hpp"
#include "Share.hpp"

class Evaluation {
private:

	double score = 0;

	Chain maxChain(const PlayerInfo& info, const int turn) {

		const auto share = Share::Get();

		Chain max;
		double chainScore = 0.0;

		constexpr int DropPos[] = { 0,1,2,3,4,5,6,7,8,9 };
		constexpr std::array<double, 5> Rate{ 1.10,1.05,1.01,1.00,0.90 };
		//const auto& dropNum = share->packSetNumber(turn);
		const int dropNum[] = { 1,2,3,4,5,6,8,9 };
		const auto& numbers = share->packNumber();

		for (const auto& pos : DropPos)
		{
			for (const auto& num : dropNum)
			{
				auto next = info.copy();
				auto chain = next.field.dropCell(pos, num);

				double score = chain.score;
				if (chain.chain >= Config::ChainIgnition)
				{
					score *= Rate[std::min(numbers[turn + 1][num], static_cast<int>(Rate.size() - 1))];
				}

				if (chainScore < score)
				{
					max = chain;
					chainScore = score;
				}
			}
		}

		return max;
	}

public:

	Evaluation() {}
	Evaluation(const PlayerInfo& info, const Chain& chain, const Evaluation& prev, const int turn) {

		auto random = Random::Get();

		const auto potentialChain = maxChain(info, turn);
		//const auto skillChain = info.copy().field.useSkill();

		const auto blockNum = info.field.countBlock();
		//const auto formNum = info.field.formCheck();

		score = prev.score;
		score -= chain.score * 10;

		score += potentialChain.chain * 1000;
		score += blockNum * 10;
		//score += formNum * 10;

		//score += skillChain.score * 10;
		//score += info.gauge / 8.0;

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
