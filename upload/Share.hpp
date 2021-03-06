#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"

class Input;

struct PlayerInfo {
	int time = 0;
	int garbage = 0;
	int gauge = 0;
	int score = 0;
	int diffScore = 0;
	Field field;

	bool input() {

		int s = score;

		std::cin >> time;
		std::cin >> garbage;
		std::cin >> gauge;
		std::cin >> score;
		field = Field::Creat();

		std::string end;
		std::cin >> end;

		diffScore = score - s;

		return end == "END";
	}

	void debug(const std::string& title = "PlayerInfo") const {

		std::cerr << title << std::endl;
		std::cerr << "time   :" << time << std::endl;
		std::cerr << "garbage:" << garbage << std::endl;
		std::cerr << "gauge  :" << gauge << std::endl;
		std::cerr << "score  :" << score << std::endl;

	}

	[[nodiscard]]
	Chain simulation(const Command& com, const Pack& pack) {

		if (com.skill)
		{
			if (garbage >= Width)
			{
				field.dropGarbage();
				garbage -= Width;
			}

			const auto chain = field.useSkill();
			score += chain.score;
			diffScore += chain.score;
			garbage -= chain.garbage;
			gauge = 0;

			return chain;
		}
		else
		{
			if (garbage >= Width)
			{
				field.dropGarbage();
				garbage -= Width;
			}

			const auto chain = field.dropPack(pack, com);
			score += chain.score;
			diffScore += chain.score;
			garbage -= chain.garbage;

			if (chain.chain > 0)
			{
				gauge = std::min(gauge + GaugeAdd, MaxGauge);
			}

			return chain;
		}

	}

	//PlayerInfo& operator=(const PlayerInfo&) = delete;

	PlayerInfo copy() const {
		return PlayerInfo(*this);
	}

};

class Share {
private:

	friend Input;

	Share() {}

	static std::shared_ptr<Share> instance;

	std::array<Pack, MaxTurn> m_packs;
	int m_turn;

	PlayerInfo m_my;
	PlayerInfo m_enemy;

	std::array<std::set<int>, MaxTurn> m_packNumbers;
	std::array<std::array<int, 10>, MaxTurn+1> m_packNumber;

public:

	void first() {

		for (int i = 0; i < MaxTurn; i++)
		{
			m_packs[i] = Pack::Create();
		}

		for (int t = 0; t < MaxTurn; t++)
		{
			for (int i = 1; i < Config::PackNumberTurn; i++)
			{
				if (t + i >= MaxTurn) break;

				const auto& pack = m_packs[t + i];
				const auto& grid = pack[0];

				m_packNumbers[t].insert(grid[0][0]);
				m_packNumbers[t].insert(grid[0][1]);
				m_packNumbers[t].insert(grid[1][0]);
				m_packNumbers[t].insert(grid[1][1]);
				m_packNumbers[t].erase(0);

			}
		}

		std::array<int, 10> number;
		for (int t = MaxTurn-1; t >= 0; t--)
		{
			std::for_each(number.begin(), number.end(), [](int& n) {
				n++;
			});

			const auto& pack = m_packs[t];
			const auto& grid = pack[0];

			number[grid[0][0]] = 0;
			number[grid[0][1]] = 0;
			number[grid[1][0]] = 0;
			number[grid[1][1]] = 0;

			m_packNumber[t] = number;
		}
		m_packNumber[MaxTurn].fill(MaxTurn);

	}

	bool loop() {

		if (!(std::cin >> m_turn)) return false;

		bool check = true;
		check &= m_my.input();
		check &= m_enemy.input();

		return check;
	}

	static void Create() { instance.reset(new Share()); }
	[[nodiscard]]
	static const std::shared_ptr<Share> Get() { return instance; }

	[[nodiscard]]
	const inline std::array<Pack, MaxTurn>& packs() const { return m_packs; }
	[[nodiscard]]
	const inline Pack& pack(const int n) const { return m_packs[n]; }
	[[nodiscard]]
	const inline Pack& pack() const { return m_packs[m_turn]; }
	[[nodiscard]]
	const inline std::array<std::set<int>, MaxTurn>& packSetNumber() const { return m_packNumbers; }
	[[nodiscard]]
	const inline std::set<int>& packSetNumber(const int n) const { return m_packNumbers[n]; }
	[[nodiscard]]
	const std::array<std::array<int, 10>, MaxTurn + 1>& packNumber() const { return m_packNumber; }

	[[nodiscard]]
	const inline int turn() const { return m_turn; }

	[[nodiscard]]
	const inline PlayerInfo& my() const { return m_my; }
	[[nodiscard]]
	const inline PlayerInfo& enemy() const { return m_enemy; }

};

class Random {
private:

	Random() {
		std::random_device rnd;
		mt.seed(rnd());
	}

	static std::shared_ptr<Random> instance;

	std::uniform_real_distribution<> range{ 0.0, 1.0 };
	std::mt19937 mt;

public:

	static void Create() { instance.reset(new Random()); }
	[[nodiscard]]
	static const std::shared_ptr<Random> Get() { return instance; }

	[[nodiscard]]
	double swing() { return range(mt); }

};
