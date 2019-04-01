#pragma once

#include <iostream>
#include <iomanip>
#include <string>

#include <random>

#include <array>
#include <vector>

#include <stack>
#include <queue>

#include <algorithm>
#include <chrono>
#include <memory>

#include <cmath>

#include "FixedGrid.hpp"
#include "Clock.hpp"

using Num = int;

constexpr int DangerLine = 2;;

constexpr int Witdh = 10;
constexpr int Height = 16 + DangerLine;

constexpr int MaxTurn = 500;
constexpr int PackSize = 2;

constexpr int PackDropRange = 8;
constexpr Num Empty = 0;
constexpr int Inf = 99;

constexpr Num Elimination = 10;

const std::string Skill = "S";

using PackArray = FixedGrid<Num, PackSize, PackSize>;
using FieldArray = FixedGrid<Num, Witdh, Height>;
using BitFieldArray = FixedGrid<bool, Witdh, Height>;

inline const Num readNum() {
	int v;
	std::cin >> v;
	return static_cast<Num>(v);
}

struct Command {
	int pos = 0;
	int rotate = 0;
	bool skill = false;

	std::string toString() const {

		if (!skill)
			return std::to_string(pos) + " " + std::to_string(rotate);

		return Skill;
	}

};

struct Chain {

	Chain(int c, int s) : chain(c), score(s) {}

	int chain = 0;
	int score = 0;
};

inline bool inside(const int& v, const int& max) { return (0 <= v && v < max); }
inline bool inside(const std::pair<int, int>& pos) { return (inside(pos.first, Witdh) && inside(pos.second, Height)); }
