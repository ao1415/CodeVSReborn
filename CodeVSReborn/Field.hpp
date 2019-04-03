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
					else if (sub > Elimination)
					{
						w = std::max(1, w - 1);
						break;
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
					else if (sub > Elimination)
					{
						h = std::max(1, h - 1);
						break;
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
			for (int i = 0; i < Height; i++)
			{
				while (inside(x + w, y + w))
				{
					const auto sub = cumulative[1][y + w][x + w] - cumulative[1][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					else if (sub > Elimination)
					{
						w = std::max(1, w - 1);
						break;
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

				x++;
				y++;

				if (!inside(x, y))
					break;

			}
		}
		for (int s = 1; s < Height; s++)
		{
			int x = 0;
			int y = s;

			int w = 1;
			for (int i = 0; i < Height; i++)
			{
				while (inside(x + w, y + w))
				{
					const auto sub = cumulative[1][y + w][x + w] - cumulative[1][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					else if (sub > Elimination)
					{
						w = std::max(1, w - 1);
						break;
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

				x++;
				y++;

				if (!inside(x, y))
					break;

			}
		}

		//左下
		for (int s = 1; s < Witdh; s++)
		{
			int x = s;
			int y = 0;

			int w = 1;
			for (int i = 0; i < Height; i++)
			{
				while (inside(x - w, y + w))
				{
					const auto sub = cumulative[3][y + w][x - w] - cumulative[3][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					else if (sub > Elimination)
					{
						w = std::max(1, w - 1);
						break;
					}
					else
					{
						for (int p = 0; p <= w; p++)
						{
							bitField[y + p][x - p] = true;
						}
						break;
					}
				}

				x--;
				y++;

				if (!inside(x, y))
					break;

			}
		}
		for (int s = 1; s < Height; s++)
		{
			int x = Witdh - 1;
			int y = s;

			int w = 1;
			for (int i = 0; i < Height; i++)
			{
				while (inside(x - w, y + w))
				{
					const auto sub = cumulative[3][y + w][x - w] - cumulative[3][y][x] + table[y][x];

					if (sub < Elimination)
					{
						w++;
					}
					else if (sub > Elimination)
					{
						w = std::max(1, w - 1);
						break;
					}
					else
					{
						for (int p = 0; p <= w; p++)
						{
							bitField[y + p][x - p] = true;
						}
						break;
					}
				}

				x--;
				y++;

				if (!inside(x, y))
					break;

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
			elevation[x] = index;
		}

	}

	void setNextField(const Pack& pack, const Command& command) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

		table[elevation[pos + 0] - 1][pos + 0] = pack[r][0][0];
		table[elevation[pos + 0] - 0][pos + 0] = pack[r][1][0];
		table[elevation[pos + 1] - 1][pos + 1] = pack[r][0][1];
		table[elevation[pos + 1] - 0][pos + 1] = pack[r][1][1];

		elevation[pos + 0] -= 2;
		elevation[pos + 1] -= 2;

		dropBlock();

	}

public:

	Field() {

		table.fill(0);

	}

	Field&& getNextField(const Pack& pack, const Command& command) const {

		Field next(*this);
		next.setNextField(pack, command);

		return std::move(next);
	}

	const Chain simulation() {

		setCumulative();

		int score = 0;
		int chain = 0;

		debug();

		while (eraseBlock())
		{
			debug();
			chain++;
			score += static_cast<int>(std::pow(1.3, chain));
			dropBlock();
			setCumulative();
		}
		debug();

		return Chain(chain, score);
	}

	void debug() const {

		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Witdh - 1; x++)
			{
				std::cerr << table[y][x] << " ";
			}
			std::cerr << table[y][Witdh - 1] << std::endl;
		}
		std::cerr << std::endl;

	}

	static Field&& Creat() {

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

