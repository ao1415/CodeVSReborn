#pragma once

#include "Base.hpp"
#include "Field.hpp"
#include "Share.hpp"

namespace Config {


	/// <summary>探査ターン数</summary>
	constexpr int Turn = 10;
	/// <summary>探査時のビーム幅</summary>
	constexpr int Chokudai = 3;
	/// <summary>探査時間</summary>
	constexpr int ThinkTime = 1000;

	/// <summary>発火ターン数</summary>
	constexpr int Ignition = 10;
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

	std::pair<Chain, double> maxChain(const PlayerInfo& info, const int turn, const int ignition) {

		const auto& packs = Share::Get()->packs();

		//両端のみ評価
		constexpr std::array<Command, 8> coms{
			Command(0,0),Command(0,1),Command(0,2),Command(0,3),
			Command(PackDropRange - 1,0),Command(PackDropRange - 1,1),Command(PackDropRange - 1,2),Command(PackDropRange - 1,3)
		};

		Chain max;
		double chainScore = 0.0;

		for (int t = 0; t < 3; t++)
		{
			if (turn + t >= MaxTurn) break;

			for (const auto& com : coms)
			{
				auto next = info.copy();
				auto chain = next.simulation(com, packs[ignition + t]);

				const double score = chain.score*Config::IgnitionRate[t];

				if (max.score < score)
				{
					max = chain;
					chainScore = score;
					this->ignition = ignition + t;
				}
			}
		}

		return std::make_pair(max, chainScore);
	}

public:

	Evaluation() {}
	Evaluation(const PlayerInfo& info, const Chain& chain, const Evaluation& prev, const int turn, const int ignition) {

		auto random = Random::Get();

		this->ignition = ignition;

		//const auto[chain, chainScore] = maxChain(info, turn, ignition);

		score = prev.score;

		const int dt = turn - ignition;
		if (0 <= dt && dt < 3)
			score += chain.score*Config::IgnitionRate[dt];
		else
			score += chain.score;

		score += chain.chain * 10;

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
