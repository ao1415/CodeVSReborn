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

	void input() {

		int s = score;

		std::cin >> time;
		std::cin >> garbage;
		std::cin >> gauge;
		std::cin >> score;
		field = Field::Creat();

		diffScore = score - s;
	}

	void debug() const {

		std::cerr << "PlayerInfo" << std::endl;
		std::cerr << "time   :" << time << std::endl;
		std::cerr << "garbage:" << garbage << std::endl;
		std::cerr << "gauge  :" << gauge << std::endl;
		std::cerr << "score  :" << score << std::endl;

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

public:

	void first() {

		for (int i = 0; i < MaxTurn; i++)
		{
			m_packs[i] = Pack::Create();
		}
	}

	void loop() {

		std::cin >> m_turn;

		m_my.input();
		m_enemy.input();

	}

	static void Create() { instance.reset(new Share()); }
	[[nodiscard]]
	static const std::shared_ptr<Share> Get() { return instance; }

	[[nodiscard]]
	const inline std::array<Pack, MaxTurn>& packs() const { return m_packs; }
	[[nodiscard]]
	const inline Pack& pack(const int n) const { return m_packs[n]; }

	[[nodiscard]]
	const inline int turn() const { return m_turn; }

	[[nodiscard]]
	const inline PlayerInfo& my() const { return m_my; }
	[[nodiscard]]
	const inline PlayerInfo& enemy() const { return m_enemy; }

};
