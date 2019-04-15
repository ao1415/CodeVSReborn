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
	Field field;
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

		std::cin >> m_my.time;
		std::cin >> m_my.garbage;
		std::cin >> m_my.gauge;
		std::cin >> m_my.score;
		m_my.field = Field::Creat();

		std::cin >> m_enemy.time;
 		std::cin >> m_enemy.garbage;
		std::cin >> m_enemy.gauge;
		std::cin >> m_enemy.score;
		m_enemy.field = Field::Creat();
	}

	static void Create() { instance.reset(new Share()); }
	static const std::shared_ptr<Share> Get() { return instance; }

	const inline std::array<Pack, MaxTurn>& packs() const { return m_packs; }
	const inline Pack& pack(const int n) const { return m_packs[n]; }

	const inline int turn() const { return m_turn; }

	const inline PlayerInfo& my() const { return m_my; }
	const inline PlayerInfo& enemy() const { return m_enemy; }

};
