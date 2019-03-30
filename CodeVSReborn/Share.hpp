#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"

class Input;

struct PlayerInfo {
	int time = 0;
	int garbage = 0;
	int gauge = 0;
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

	static void Create() { instance.reset(new Share()); }
	static Share& Get() { return *instance; }

	const inline std::array<Pack, MaxTurn>& packs() const { return m_packs; }
	const inline Pack& pack(const int n) const { return m_packs[n]; }

	const inline int turn() const { return m_turn; }

	const inline PlayerInfo my() const { return m_my; }
	const inline PlayerInfo enemy() const { return m_enemy; }

};

class Input {
private:



public:

	void first() {

		auto share = Share::Get();

		for (int i = 0; i < MaxTurn; i++)
		{
			share.m_packs[i] = Pack::Create();
		}

	}

	void loop() {

		auto share = Share::Get();

		std::cin >> share.m_turn;

		std::cin >> share.m_my.time;
		std::cin >> share.m_my.gauge;
		std::cin >> share.m_my.garbage;
		share.m_my.field = Field::Creat();

		std::cin >> share.m_enemy.time;
		std::cin >> share.m_enemy.gauge;
		std::cin >> share.m_enemy.garbage;
		share.m_enemy.field = Field::Creat();
	}

};
