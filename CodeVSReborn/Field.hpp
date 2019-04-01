#pragma once

#include "Base.hpp"
#include "Pack.hpp"

class Field {
private:

	FieldArray table;
	//横・右下・下・左下
	std::array<FieldArray, 4> cumulative;
	std::array<Num, Witdh> elevation;

	void setElevation() {

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

	void setCumulative() {

		int dx[] = { 1,1,0,-1 };
		int dy[] = { 0,1,1,1 };

		for (int y = 0; y < Height; y++)
		{
			for (int d = 0; d < 4; d++)
				cumulative[d][y][0] = table[y][0];
		}
		for (int x = 1; x < Witdh; x++)
		{
			for (int d = 0; d < 4; d++)
				cumulative[d][0][x] = table[0][x];
		}

		for (int y = 1; y < Height; y++)
		{
			for (int x = 1; x < Witdh; x++)
			{
				for (int d = 0; d < 4; d++)
				{
					int px = x - dx[d];
					int py = y - dy[d];

					if (table[y][x] != Empty)
					{
						cumulative[d][y][x] = cumulative[d][py][px] + table[y][x];
					}
					else
					{
						cumulative[d][y][x] = cumulative[d][py][px] + Inf;
					}
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

		//横
		for (int y = 0; y < Height; y++)
		{
			int w = 1;
			for (int x = 0; x < Witdh; x++)
			{
				while (x + w < Witdh)
				{
					const auto sub = cumulative[0][y][x + w] - cumulative[0][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					if (sub > Elimination)
					{
						w = std::max(1, w - 1);
					}
					else
					{
						for (int p = x; p <= x + w; p++)
						{
							bitField[y][p] = true;
						}
						break;
					}
				}
			}
		}

		//横
		for (int x = 0; x < Witdh; x++)
		{
			int h = 1;
			for (int y = elevation[x]; y < Height; y++)
			{
				while (y + h < Height)
				{
					const auto sub = cumulative[2][y + h][x] - cumulative[2][y][x] + table[y][x];

					if (sub < Elimination)
					{
						h++;
					}
					if (sub > Elimination)
					{
						h = std::max(1, h - 1);
					}
					else
					{
						for (int p = y; p <= y + h; p++)
						{
							bitField[p][x] = true;
						}
						break;
					}
				}
			}
		}

		//右下
		for (int s = 0; s < Witdh; s++)
		{
			int x = s;
			int y = 0;

			int w = 1;
			for (int i; i < Height; i++)
			{
				x++;
				y++;
				while (inside(x + w, y + w))
				{
					const auto sub = cumulative[1][y + w][x + w] - cumulative[1][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					if (sub > Elimination)
					{
						w = std::max(1, w - 1);
					}
					else
					{
						for (int p = 0; p <= w; p++)
						{
							bitField[y + p][x + p] = true;
						}
						break;
					}
				}
			}
		}
		for (int s = 1; s < Height; s++)
		{
			int x = 0;
			int y = s;

			int w = 1;
			for (int i; i < Height; i++)
			{
				x++;
				y++;
				while (inside(x + w, y + w))
				{
					const auto sub = cumulative[1][y + w][x + w] - cumulative[1][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					if (sub > Elimination)
					{
						w = std::max(1, w - 1);
					}
					else
					{
						for (int p = 0; p <= w; p++)
						{
							bitField[y + p][x + p] = true;
						}
						break;
					}
				}
			}
		}

		//左下
		for (int s = 1; s < Witdh; s++)
		{
			int x = s;
			int y = 0;

			int w = 1;
			for (int i; i < Height; i++)
			{
				x--;
				y++;
				while (inside(x + w, y + w))
				{
					const auto sub = cumulative[1][y + w][x + w] - cumulative[1][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					if (sub > Elimination)
					{
						w = std::max(1, w - 1);
					}
					else
					{
						for (int p = 0; p <= w; p++)
						{
							bitField[y + p][x + p] = true;
						}
						break;
					}
				}
			}
		}
		for (int s = 1; s < Height; s++)
		{
			int x = Witdh - 1;
			int y = s;

			int w = 1;
			for (int i; i < Height; i++)
			{
				x++;
				y++;
				while (inside(x + w, y + w))
				{
					const auto sub = cumulative[1][y + w][x + w] - cumulative[1][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					if (sub > Elimination)
					{
						w = std::max(1, w - 1);
					}
					else
					{
						for (int p = 0; p <= w; p++)
						{
							bitField[y + p][x + p] = true;
						}
						break;
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

	void dropBlock() {

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
		}

	}

public:

	Field() {

		table.fill(0);

	}

	void setNextField(const Pack& pack, const Command& command) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

		table[elevation[pos] - 1][pos + 0] = pack[r][0][0];
		table[elevation[pos] - 0][pos + 0] = pack[r][1][0];
		table[elevation[pos] - 1][pos + 1] = pack[r][0][1];
		table[elevation[pos] - 0][pos + 1] = pack[r][1][1];

		elevation[pos + 0] += 2;
		elevation[pos + 1] += 2;

		setCumulative();
	}

	Field&& getNextField(const Pack& pack, const Command& command) {

		Field next;
		next.setNextField(pack, command);

		return std::move(next);
	}

	const Chain simulation() {

		setCumulative();

		int score = 0;
		int chain = 1;

		while (eraseBlock())
		{
			score += static_cast<int>(std::pow(1.3, chain));
			chain++;
			dropBlock();
		}

		return Chain(chain, score);
	}

	static Field&& Creat() {

		Field field;

		for (int y = DangerLine; y < Height; y++)
		{
			for (int x = 0; x < Witdh; x++)
			{
				field.table[y][x] = readNum();
			}
		}

		std::string end;
		std::cin >> end;

		field.setElevation();

		return std::move(field);
	}

};

