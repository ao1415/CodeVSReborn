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

constexpr int DangerLine = 3;

constexpr int Witdh = 10;
constexpr int Height = 16 + DangerLine;

constexpr int MaxTurn = 500;
constexpr int PackSize = 2;

constexpr int PackDropRange = 8;
constexpr Num Empty = 0;
constexpr Num Inf = 99;

constexpr int SkillCost = 80;

constexpr Num Elimination = 10;
constexpr Num Garbage = 11;

const std::string Skill = "S";

using PackArray = FixedGrid<Num, PackSize, PackSize>;
using FieldArray = FixedGrid<Num, Witdh, Height>;
using BitFieldArray = FixedGrid<bool, Witdh, Height>;

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

	Chain() : Chain(0, 0, 0) {}
	Chain(int c, int s, int g) : chain(c), score(s), garbage(g) {}

	int chain = 0;
	int score = 0;
	int garbage = 0;
};

inline bool _inside(const int& v, const int& max) { return (0 <= v && v < max); }
inline bool inside(const int& x, const int& y) { return (_inside(x, Witdh) && _inside(y, Height)); }
inline bool inside(const std::pair<int, int>& pos) { return inside(pos.first, pos.second); }
