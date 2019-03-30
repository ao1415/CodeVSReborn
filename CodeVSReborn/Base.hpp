#pragma once

#include <iostream>
#include <string>

#include <random>

#include <array>
#include <vector>

#include <stack>
#include <queue>

#include <algorithm>
#include <chrono>
#include <memory>

#include "FixedGrid.hpp"
#include "Clock.hpp"

constexpr int DangerLine = 2;;

constexpr int Witdh = 10;
constexpr int Height = 16 + 2;

constexpr int MaxTurn = 500;
constexpr int PackSize = 2;

constexpr int PackDropRange = 8;

const std::string Skill = "S";

using Num = int;

using PackArray = FixedGrid<Num, PackSize, PackSize>;
using FieldArray = FixedGrid<Num, Witdh, Height>;

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
