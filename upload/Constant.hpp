#pragma once

#include <string>

using Num = char;

constexpr int DangerLine = 3;

constexpr int Width = 10;
constexpr int Height = 16 + DangerLine;

constexpr int MaxTurn = 500;
constexpr int MaxGauge = 100;
constexpr int PackSize = 2;

constexpr int PackDropRange = 9;
constexpr Num Empty = 0;
constexpr Num Inf = 99;

constexpr int SkillCost = 80;
constexpr int GaugeAdd = 8;

constexpr Num Elimination = 10;
constexpr Num Garbage = 11;

const std::string Skill = "S";

using HashBit = unsigned long long int;

//下位ビットが左端、上位ビットが右端
constexpr std::array<uint16_t, Width + 1> RecalcLine
{
	0b0000000001,
	0b0000000011,
	0b0000000110,
	0b0000001100,
	0b0000011000,
	0b0000110000,
	0b0001100000,
	0b0011000000,
	0b0110000000,
	0b1100000000,
	0b1111111111,
};

constexpr std::array<uint16_t,Width> RecalcBitMask
{
	0b0000000001,
	0b0000000010,
	0b0000000100,
	0b0000001000,
	0b0000010000,
	0b0000100000,
	0b0001000000,
	0b0010000000,
	0b0100000000,
	0b1000000000,
};

constexpr std::array<uint32_t, Height> LineBit
{
	0x00000001,
	0x00000002,
	0x00000004,
	0x00000008,
	0x00000010,
	0x00000020,
	0x00000040,
	0x00000080,
	0x00000100,
	0x00000200,
	0x00000400,
	0x00000800,
	0x00001000,
	0x00002000,
	0x00004000,
	0x00008000,
	0x00010000,
	0x00020000,
	0x00040000
};
