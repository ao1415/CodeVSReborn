#pragma once

#include "Base.hpp"
#include "Field.hpp"
#include "Share.hpp"

namespace Config {


	/// <summary>探査ターン数</summary>
	constexpr int Turn = 15;
	/// <summary>探査時のビーム幅</summary>
	constexpr int Chokudai = 3;
	/// <summary>探査時間</summary>
	constexpr int ThinkTime = 1000;

	/// <summary>発火ターン数</summary>
	constexpr int Ignition = 15;
	/// <summary>発火ターンのずれ</summary>
	constexpr int IgnitionDev = 3;
	constexpr std::array<double, IgnitionDev> IgnitionRate{ 1.5,1.3,1.1 };


	/// <summary>敵の探査ターン数</summary>
	constexpr int EnemyThinkTurn = 3;

}

class Evaluation {
private:

	double score = 0;
	int ignition = 0;

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
	Evaluation(const PlayerInfo& info, const Chain& chain, const Evaluation& prev, const int turn, const int ignition) {

		auto random = Random::Get();

		this->ignition = ignition;

		const auto potentialChain = maxChain(info);

		score = prev.score;
		score += chain.score;

		score += potentialChain.score * 2;

		score += random->swing();
	}

	bool operator<(const Evaluation& other) const {

		return score < other.score;
	}

	int getIgnition() const { return ignition; }

	void debug() const {

		std::cerr << "Evaluation" << std::endl;
		std::cerr << "Evaluation:" << score << std::endl;
		std::cerr << "Ignition  :" << ignition << std::endl;

	}

};
