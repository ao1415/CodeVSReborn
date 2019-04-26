#pragma once

#include "Base.hpp"
#include "Pack.hpp"

constexpr std::array<std::bitset<Width>, Width> LineColumn
{
	0b1100000000,
	0b1110000000,
	0b0111000000,
	0b0011100000,
	0b0001110000,
	0b0000111000,
	0b0000011100,
	0b0000001110,
	0b0000000111,
	0b0000000011
};
constexpr std::array<std::bitset<Width>, PackDropRange> PackColumn
{
	0b1110000000,
	0b1111000000,
	0b0111100000,
	0b0011110000,
	0b0001111000,
	0b0000111100,
	0b0000011110,
	0b0000001111,
	0b0000000111
};

using BitTable = std::bitset<256>;
constexpr std::array<BitTable, Height> BlockTable
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
const std::array<BitTable, 8> BlockSentinel
{
	//上
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x3FFFF7FF) << 160) |
	(BitTable(0xFEFFFFDF) << 128) |
	(BitTable(0xFFFBFFFF) << 96) |
	(BitTable(0x7FFFEFFF) << 64) |
	(BitTable(0xFDFFFFBF) << 32) |
	(BitTable(0xFFF7FFFE)),
	//下
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x1FFFFBFF) << 160) |
	(BitTable(0xFF7FFFEF) << 128) |
	(BitTable(0xFFFDFFFF) << 96) |
	(BitTable(0xBFFFF7FF) << 64) |
	(BitTable(0xFEFFFFDF) << 32) |
	(BitTable(0xFFFBFFFF)),
	//右
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x3FFFFFFF) << 160) |
	(BitTable(0xFFFFFFFF) << 128) |
	(BitTable(0xFFFFFFFF) << 96) |
	(BitTable(0xFFFFFFFF) << 64) |
	(BitTable(0xFFFFFFFF) << 32) |
	(BitTable(0xFFF80000)),
	//左
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x000007FF) << 160) |
	(BitTable(0xFFFFFFFF) << 128) |
	(BitTable(0xFFFFFFFF) << 96) |
	(BitTable(0xFFFFFFFF) << 64) |
	(BitTable(0xFFFFFFFF) << 32) |
	(BitTable(0xFFFFFFFF)),
	//右上
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x3FFFF7FF) << 160) |
	(BitTable(0xFEFFFFDF) << 128) |
	(BitTable(0xFFFBFFFF) << 96) |
	(BitTable(0x7FFFEFFF) << 64) |
	(BitTable(0xFDFFFFBF) << 32) |
	(BitTable(0xFFF00000)),
	//右下
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x1FFFFBFF) << 160) |
	(BitTable(0xFF7FFFEF) << 128) |
	(BitTable(0xFFFDFFFF) << 96) |
	(BitTable(0xBFFFF7FF) << 64) |
	(BitTable(0xFEFFFFDF) << 32) |
	(BitTable(0xFFF80000)),
	//左上
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x000007FF) << 160) |
	(BitTable(0xFEFFFFDF) << 128) |
	(BitTable(0xFFFBFFFF) << 96) |
	(BitTable(0x7FFFEFFF) << 64) |
	(BitTable(0xFDFFFFBF) << 32) |
	(BitTable(0xFFF7FFFE)),
	//左下
	(BitTable(0x00000000) << 224) |
	(BitTable(0x00000000) << 192) |
	(BitTable(0x000003FF) << 160) |
	(BitTable(0xFF7FFFEF) << 128) |
	(BitTable(0xFFFDFFFF) << 96) |
	(BitTable(0xBFFFF7FF) << 64) |
	(BitTable(0xFEFFFFDF) << 32) |
	(BitTable(0xFFFBFFFF))
};

class Field {
private:

	FieldArray table;

	using CheckLine = std::bitset<Width>;

	std::array<Num, Width> elevation;

	void setElevation() {

		elevation.fill(Height - 1);
		for (int x = 0; x < Width; x++)
		{
			for (int y = DangerLine; y < Height; y++)
			{
				if (table[y][x] != Empty)
				{
					elevation[x] = y - 1;
					break;
				}
			}
		}

	}

	bool eraseBlock(CheckLine& recalc) {

		BitFieldArray bitField(false);

		for (int x = 0; x < Width; x++)
		{
			if (recalc[x])
			{
				for (int y = Height - 1; y > elevation[x]; y--)
				{
					//上
					{
						int px = x + 0; int py = y - 1;

						const int sum = table[y][x] + table[py][px];
						if (sum == Elimination)
							bitField[y][x] = bitField[py][px] = true;
					}

					//右
					{
						int px = x + 1; int py = y - 0;

						if (px < Width)
						{
							const int sum = table[y][x] + table[py][px];
							if (sum == Elimination)
								bitField[y][x] = bitField[py][px] = true;
						}
					}

					//右上
					{
						int px = x + 1; int py = y - 1;

						if (px < Width)
						{
							const int sum = table[y][x] + table[py][px];
							if (sum == Elimination)
								bitField[y][x] = bitField[py][px] = true;
						}
					}

					//左上
					{
						int px = x - 1; int py = y - 1;

						if (0 <= px)
						{
							const int sum = table[y][x] + table[py][px];
							if (sum == Elimination)
								bitField[y][x] = bitField[py][px] = true;
						}
					}
				}
			}
		}

		const auto prev = recalc;

		for (int x = 0; x < Width; x++)
		{
			bool update = false;
			if (prev[x])
			{
				for (int y = Height - 1; y > elevation[x]; y--)
				{
					if (bitField[y][x])
					{
						table[y][x] = Empty;

						update = true;
					}
				}
				recalc.set(x, update);
			}
		}

		return recalc.any();
	}

	const Chain bombBlock(CheckLine& recalc) {

		const Num BombNumber = 5;

		BitTable bombTable(false);
		BitTable blockTable(false);

		//ビットボードの作成
		for (int x = Width - 1; x >= 0; x--)
		{
			BitTable bombLine(false);
			BitTable blockLine(false);

			for (int y = 0; y < Height; y++)
			{
				if (table[y][x] == BombNumber)
				{
					bombLine |= BlockTable[y];
				}
				else if (table[y][x] != Empty && table[y][x] != Garbage)
				{
					blockLine |= BlockTable[y];
				}
			}

			bombTable <<= Height;
			bombTable |= bombLine;

			blockTable <<= Height;
			blockTable |= blockLine;
		}

		BitTable bombBoard(false);
		
		bombBoard |= bombTable;
		//上
		{
			auto sentinel = bombTable & BlockSentinel[0];
			auto bomb = blockTable & (sentinel >> 1);
			bombBoard |= bomb;
		}
		//下
		{
			auto sentinel = bombTable & BlockSentinel[1];
			auto bomb = blockTable & (sentinel << 1);
			bombBoard |= bomb;
		}
		//右
		{
			auto sentinel = bombTable & BlockSentinel[2];
			auto bomb = blockTable & (sentinel >> Height);
			bombBoard |= bomb;
		}
		//左
		{
			auto sentinel = bombTable & BlockSentinel[3];
			auto bomb = blockTable & (sentinel << Height);
			bombBoard |= bomb;
		}
		//右上
		{
			auto sentinel = bombTable & BlockSentinel[4];
			auto bomb = blockTable & (sentinel >> Height + 1);
			bombBoard |= bomb;
		}
		//右下
		{
			auto sentinel = bombTable & BlockSentinel[5];
			auto bomb = blockTable & (sentinel >> Height - 1);
			bombBoard |= bomb;
		}
		//左上
		{
			auto sentinel = bombTable & BlockSentinel[6];
			auto bomb = blockTable & (sentinel << Height - 1);
			bombBoard |= bomb;
		}
		//左下
		{
			auto sentinel = bombTable & BlockSentinel[7];
			auto bomb = blockTable & (sentinel << Height + 1);
			bombBoard |= bomb;
		}

		int disBlock = static_cast<int>(bombBoard.count());
		for (int x = 0; x < Width; x++)
		{
			bool update = false;
			for (int y = Height - 1; y > elevation[x]; y--)
			{
				if (bombBoard[y + x * Height])
				{
					table[y][x] = Empty;

					update = true;
				}
			}
			recalc.set(x, update);
		}

		int score = 0;
		if (disBlock != 0)
			score = static_cast<int>(25.0 * pow(2.0, disBlock / 12.0));

		return Chain(0, score, score / 2);
	}

	const Chain chainBlock(CheckLine& recalc) {

		int score = 0;
		int chain = 0;
		double pow = 1.0;

		while (eraseBlock(recalc))
		{
			chain++;
			pow *= 1.3;
			score += static_cast<int>(pow);
			fallBlock(recalc);
		}

		return Chain(chain, score, score / 2);
	}

	void fallBlock(CheckLine& recalc) {

		const auto prev = recalc;
		recalc.reset();

		for (int x = 0; x < Width; x++)
		{
			if (prev[x])
			{
				const int h = elevation[x];
				int index = Height - 1;
				for (int y = index; y > h; y--)
				{
					if (table[y][x] != Empty)
					{
						if (index != y)
						{
							table[index][x] = table[y][x];
							table[y][x] = Empty;
						}
						index--;
					}
				}
				elevation[x] = index;
				recalc |= LineColumn[x];
			}
		}

	}

	void setPack(const Pack& pack, const Command& command, CheckLine& recalc) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

		recalc = PackColumn[pos];

		if (pack[r][1][0] != Empty)
		{
			table[elevation[pos + 0]][pos + 0] = pack[r][1][0];
			elevation[pos + 0]--;
		}
		if (pack[r][0][0] != Empty)
		{
			table[elevation[pos + 0]][pos + 0] = pack[r][0][0];
			elevation[pos + 0]--;
		}

		if (pack[r][1][1] != Empty)
		{
			table[elevation[pos + 1]][pos + 1] = pack[r][1][1];
			elevation[pos + 1]--;
		}
		if (pack[r][0][1] != Empty)
		{
			table[elevation[pos + 1]][pos + 1] = pack[r][0][1];
			elevation[pos + 1]--;
		}

	}

public:

	Field() {
		table.fill(0);
		elevation.fill(Height - 1);
	}

	[[nodiscard]]
	Field copy() const { return std::move(Field(*this)); }

	[[nodiscard]]
	const Chain dropPack(const Pack& pack, const Command& command) {

		CheckLine recalc(false);

		setPack(pack, command, recalc);

		const auto chain = chainBlock(recalc);

		//debug();

		return chain;
	}

	[[nodiscard]]
	const Chain useSkill() {

		CheckLine recalc(false);

		const auto bomb = bombBlock(recalc);

		fallBlock(recalc);

		const auto chain = chainBlock(recalc);

		//debug();

		return bomb + chain;
	}

	const Chain dropCell(const int num) {

		CheckLine recalc(false);

		constexpr int pos = 2;

		recalc = LineColumn[pos];

		table[elevation[pos]][pos] = num;
		elevation[pos]--;

		const auto chain = chainBlock(recalc);

		return chain;
	}

	void dropGarbage() {

		for (int x = 0; x < Width; x++)
		{
			table[elevation[x]][x] = Garbage;
			elevation[x]--;
		}

	}

	[[nodiscard]]
	const FieldArray& getFieldArray() const { return table; }
	[[nodiscard]]
	const bool isSurvival() const {
		return !std::any_of(elevation.cbegin(), elevation.cend(), [](int x) {
			return x <= DangerLine;
		});
	}


	//[[deprecated("used for debug only")]]
	void debug() const {

		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				//std::cerr << (table[y][x] < Elimination ? std::to_string(table[y][x]) : "#") << " ";
				std::cerr << static_cast<int>(table[y][x]) << " ";
			}
			std::cerr << std::endl;
		}

	}

	[[nodiscard]]
	static Field Creat() {

		Field field;

		for (int y = DangerLine; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				int num;
				std::cin >> num;
				field.table[y][x] = static_cast<Num>(num);
			}
			std::cin.ignore();
		}

		field.setElevation();

		return std::move(field);
	}

};

