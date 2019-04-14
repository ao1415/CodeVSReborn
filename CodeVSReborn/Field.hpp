#pragma once

#include "Base.hpp"
#include "Pack.hpp"

class Field {
private:

	FieldArray table;
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

	bool eraseBlock() {

		int count = 0;

		//横・右下・下・左下
		int dx[] = { 1,1,0,-1 };
		int dy[] = { 0,1,1,1 };

		BitFieldArray bitField(false);

		for (int y = 0; y < Height - 1; y++)
		{
			for (int x = 0; x < Witdh - 1; x++)
			{
				for (int d = 0; d < 4; d++)
				{
					int px = x + dx[d];
					int py = y + dy[d];

					const int sum = table[y][x] + table[py][px];

					if (sum == Elimination)
					{
						bitField[y][x] = true;
						bitField[py][px] = true;
					}
				}
			}
		}

		bool isErase = false;
		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Witdh; x++)
			{
				if (bitField[y][x])
				{
					table[y][x] = Empty;
					isErase = true;
				}
			}
		}

		return isErase;
	}

	const Chain bombBlock() {

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
							if (table[py][px] != Garbage)
							{
								bitField[py][px] = true;
							}
						}
					}
				}
				else if (table[y][x] != Garbage)
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
		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Witdh; x++)
			{
				if (bitField[y][x])
				{
					table[y][x] = Empty;
					disBlock++;
				}
			}
		}
		int score = static_cast<int>(25.0 * pow(2.0, disBlock / 12.0));

		return Chain(0, score, score / 2);
	}
	const Chain chainBlock() {

		int score = 0;
		int chain = 0;

		while (eraseBlock())
		{
			chain++;
			score += static_cast<int>(std::pow(1.3, chain));
			fallBlock();
		}

		return Chain(chain, score, score / 2);
	}

	void fallBlock() {

		for (int x = 0; x < Witdh; x++)
		{
			const int h = elevation[x];
			int index = Height - 1;

			for (int y = Height - 1; y >= h; y--)
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

	void setPack(const Pack& pack, const Command& command) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

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

	Field copy() const { return std::move(Field(*this)); }

	const Chain dropPack(const Pack& pack, const Command& command) {

		setPack(pack, command);

		const auto chain = chainBlock();

		//debug();

		return chain;
	}

	const Chain useSkill() {

		const auto bomb = bombBlock();
		fallBlock();

		const auto chain = chainBlock();

		//debug();

		return Chain(bomb.chain + chain.chain, bomb.score + chain.score, bomb.garbage + chain.garbage);
	}

	const FieldArray& getFieldArray() const { return table; }
	
	[[deprecated("used for debug only")]]
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

		std::string end;
		std::cin >> end;

		field.setElevation();

		return std::move(field);
	}

};

