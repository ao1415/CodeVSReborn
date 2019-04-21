#pragma once

#include "Base.hpp"
#include "Pack.hpp"

class Field {
private:

	FieldArray table;
	std::array<Num, Witdh> elevation;

	bool survival = true;

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

	bool eraseBlock(std::array<Num, Witdh>& recalc) {

		BitFieldArray bitField(false);

		for (int x = 0; x < Witdh; x++)
		{
			for (int y = recalc[x]; y > elevation[x]; y--)
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

		const auto diff = recalc;
		recalc = elevation;

		bool isErase = false;
		for (int x = 0; x < Witdh; x++)
		{
			bool update = false;
			for (int y = diff[x]; y > elevation[x]; y--)
			{
				if (bitField[y][x])
				{
					table[y][x] = Empty;
					isErase = true;

					if (!update)
					{
						int low = std::min(y + 1, Height - 1);
						recalc[x - 0] = std::max(recalc[x - 0], low);
						if (0 <= x - 1)
							recalc[x - 1] = std::max(recalc[x - 1], low);
						if (x + 1 < Witdh)
							recalc[x + 1] = std::max(recalc[x + 1], low);
						update = true;
					}
				}
			}
		}

		return isErase;
	}

	void checkDangerLine() {
		survival = !std::any_of(elevation.cbegin(), elevation.cend(), [](int x) {
			return x <= DangerLine;
		});
	}

	const Chain bombBlock(std::array<Num, Witdh>& recalc) {

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
						int low = std::min(y + 1, Height - 1);
						recalc[x - 0] = std::max(recalc[x - 0], low);
						if (0 <= x - 1)
							recalc[x - 1] = std::max(recalc[x - 1], low);
						if (x + 1 < Witdh)
							recalc[x + 1] = std::max(recalc[x + 1], low);
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
	const Chain chainBlock(std::array<Num, Witdh>& recalc) {

		int score = 0;
		int chain = 0;

		while (eraseBlock(recalc))
		{
			chain++;
			score += static_cast<int>(std::pow(1.3, chain));
			fallBlock(recalc);
		}

		checkDangerLine();

		return Chain(chain, score, score / 2);
	}

	void fallBlock(std::array<Num, Witdh>& recalc) {

		for (int x = 0; x < Witdh; x++)
		{
			const int h = elevation[x];
			int index = recalc[x];

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

	void setPack(const Pack& pack, const Command& command, std::array<Num, Witdh>& recalc) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

		int right = std::min(pos + 2, Witdh - 1);
		int left = std::max(0, pos - 1);

		recalc[right] = std::min(elevation[right] + 1, Height - 1);
		recalc[left] = std::min(elevation[left] + 1, Height - 1);
		recalc[pos + 0] = std::min(elevation[pos + 0] + 1, Height - 1);
		recalc[pos + 1] = std::min(elevation[pos + 1] + 1, Height - 1);

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

		auto recalc = elevation;

		setPack(pack, command, recalc);

		const auto chain = chainBlock(recalc);

		//debug();

		checkDangerLine();
		return chain;
	}

	[[nodiscard]]
	const Chain useSkill() {

		auto recalc = elevation;

		const auto bomb = bombBlock(recalc);

		fallBlock(recalc);

		const auto chain = chainBlock(recalc);

		//debug();

		checkDangerLine();
		return bomb + chain;
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
	const bool isSurvival() const { return survival; }


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
				std::cin >> field.table[y][x];
			}
			std::cin.ignore();
		}

		field.setElevation();

		return std::move(field);
	}

};

