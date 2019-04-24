#pragma once

#include "Base.hpp"
#include "Pack.hpp"

class Field {
private:

	FieldArray table;

	using CheckLine = std::bitset<Witdh>;

	std::array<Num, Witdh> elevation;

	void setElevation() {

		elevation.fill(Height - 1);
		for (int x = 0; x < Witdh; x++)
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

		for (int x = 0; x < Witdh; x++)
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

						if (px < Witdh)
						{
							const int sum = table[y][x] + table[py][px];
							if (sum == Elimination)
								bitField[y][x] = bitField[py][px] = true;
						}
					}

					//右上
					{
						int px = x + 1; int py = y - 1;

						if (px < Witdh)
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
		recalc.reset();

		bool isErase = false;
		for (int x = 0; x < Witdh; x++)
		{
			bool update = false;
			if (prev[x])
			{
				for (int y = Height - 1; y > elevation[x]; y--)
				{
					if (bitField[y][x])
					{
						table[y][x] = Empty;
						isErase = true;

						if (!update)
						{
							recalc.set(x);
							if (0 <= x - 1) recalc.set(x - 1);
							if (x + 1 < Witdh) recalc.set(x + 1);

							update = true;
						}
					}
				}
			}
		}

		return isErase;
	}

	const Chain bombBlock(CheckLine& recalc) {

		const Num BombNumber = 5;

		BitFieldArray bitField(false);

		//横・右下・下・左下
		int dx[] = { 1,1,0,-1 };
		int dy[] = { 0,1,1,1 };

		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Witdh; x++)
			{
				if (table[y][x] == BombNumber)
				{
					bitField[y][x] = true;
					for (int d = 0; d < 4; d++)
					{
						int px = x + dx[d];
						int py = y + dy[d];

						if (inside(px, py))
						{
							if (table[py][px] != Garbage && table[py][px] != Empty)
							{
								bitField[py][px] = true;
							}
						}
					}
				}
				else if (table[y][x] != Garbage && table[y][x] != Empty)
				{
					for (int d = 0; d < 4; d++)
					{
						int px = x + dx[d];
						int py = y + dy[d];

						if (inside(px, py))
						{
							if (table[py][px] == BombNumber)
							{
								bitField[y][x] = true;
								bitField[py][px] = true;
								break;
							}
						}
					}
				}
			}
		}

		int disBlock = 0;
		recalc.reset();
		for (int x = 0; x < Witdh; x++)
		{
			bool update = false;
			for (int y = Height - 1; y > elevation[x]; y--)
			{
				if (bitField[y][x])
				{
					table[y][x] = Empty;
					disBlock++;

					if (!update)
					{
						recalc.set(x);
						if (0 <= x - 1) recalc.set(x - 1);
						if (x + 1 < Witdh) recalc.set(x + 1);

						update = true;
					}
				}
			}
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

		for (int x = 0; x < Witdh; x++)
		{
			const int h = elevation[x];

			if (recalc[x])
			{
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
			}
		}

	}

	void setPack(const Pack& pack, const Command& command, CheckLine& recalc) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

		int left = std::max(0, pos - 1);
		int right = std::min(pos + 2, Witdh - 1);

		recalc.set(left);
		recalc.set(pos + 0);
		recalc.set(pos + 1);
		recalc.set(right);

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

		constexpr int left = std::max(0, pos - 1);
		constexpr int right = std::min(pos + 1, Witdh - 1);

		recalc.set(right);
		recalc.set(pos);
		recalc.set(left);

		table[elevation[pos]][pos] = num;
		elevation[pos]--;

		const auto chain = chainBlock(recalc);

		return chain;
	}

	void dropGarbage() {

		for (int x = 0; x < Witdh; x++)
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
			for (int x = 0; x < Witdh; x++)
			{
				//std::cerr << (table[y][x] < Elimination ? std::to_string(table[y][x]) : "#") << " ";
				std::cerr << table[y][x] << " ";
			}
			std::cerr << std::endl;
		}

	}

	[[nodiscard]]
	static Field Creat() {

		Field field;

		for (int y = DangerLine; y < Height; y++)
		{
			for (int x = 0; x < Witdh; x++)
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

