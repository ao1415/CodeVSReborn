#pragma once

#include <iostream>
#include <iomanip>
#include <string>

#include <random>

#include <array>
#include <vector>
#include <bitset>

#include <stack>
#include <queue>
#include <unordered_set>

#include <algorithm>
#include <chrono>
#include <memory>

#include <cmath>
#include <tuple>

#include "Constant.hpp"

#include "FixedGrid.hpp"
#include "Clock.hpp"

using PackArray = FixedGrid<Num, PackSize, PackSize>;
using FieldArray = FixedGrid<Num, Width, Height>;
using BitFieldArray = FixedGrid<bool, Width, Height>;

struct Command {
	int pos = 0;
	int rotate = 0;
	bool skill = false;

	constexpr Command() : Command(0, 0, false) {}
	constexpr Command(int p, int r) : Command(p, r, false) {}
	constexpr Command(bool s) : Command(0, 0, s) {}
	constexpr Command(int p, int r, bool s) {
		pos = p;
		rotate = r;
		skill = s;
	}

	[[nodiscard]]
	const std::string toString() const {

		if (!skill)
			return std::to_string(pos) + " " + std::to_string(rotate);

		return Skill;
	}

};

struct Chain {

	Chain() : Chain(0, 0, 0) {}
	Chain(int c, int s, int g) : chain(c), score(s), garbage(g) {}

	[[nodiscard]]
	Chain operator+(const Chain& other) const {
		return Chain(chain + other.chain, score + other.score, garbage + other.garbage);
	}
	void operator+=(const Chain& other) {
		chain += other.chain;
		score += other.score;
		garbage += other.garbage;
	}

	bool operator<(const Chain& other) const {
		return score < other.score;
	}

	void debug() const {
		std::cerr << "Chain" << std::endl;
		std::cerr << "chain  :" << chain << std::endl;
		std::cerr << "score  :" << score << std::endl;
		std::cerr << "garbage:" << garbage << std::endl;
	}

	int chain = 0;
	int score = 0;
	int garbage = 0;
};

inline bool _inside(const int& v, const int& max) { return (0 <= v && v < max); }
inline bool inside(const int& x, const int& y) { return (_inside(x, Width) && _inside(y, Height)); }
inline bool inside(const std::pair<int, int>& pos) { return inside(pos.first, pos.second); }
