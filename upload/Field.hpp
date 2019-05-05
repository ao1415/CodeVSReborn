#pragma once

#include "Base.hpp"
#include "Pack.hpp"

class Field {
private:

	/*
	内部データの方針
	x座標：見た目通り　フィールドの左端が0、右端が9
	y座標：下から　フィールドの下が下位ビット、上が上位ビット
	0：ブロックがある場合1、空白の場合0
	1～9,11：各数字がその座標にある場合1、なければ0
	*/
	std::array<std::array<uint32_t, Width>, 12> table;

	using CheckLine = std::bitset<Width>;

	//最も上のブロックのy座標+1の値を保持する
	std::array<Num, Width> elevation;

	void setElevation() {

		for (int x = 0; x < Width; x++)
		{
			setElevation(x);
		}

	}
	void setElevation(const int x) {

		auto line = ~table[0][x];
		for (int i = 0; i < Height; i++)
		{
			if (line >> i & 1)
			{
				elevation[x] = i;
				break;
			}
		}

	}

	bool eraseBlock(uint16_t& recalc) {

		std::array<uint32_t, Width> chainTable = { 0,0,0,0,0,0,0,0,0,0 };

		for (int x = 0; x < Width; x++)
		{
			if ((recalc&RecalcBitMask[x]) > 0)
			{
				//上
				uint32_t u = 0;
				u |= (table[1][x] << 1)&table[9][x];
				u |= (table[9][x] << 1)&table[1][x];

				u |= (table[2][x] << 1)&table[8][x];
				u |= (table[8][x] << 1)&table[2][x];

				u |= (table[3][x] << 1)&table[7][x];
				u |= (table[7][x] << 1)&table[3][x];

				u |= (table[4][x] << 1)&table[6][x];
				u |= (table[6][x] << 1)&table[4][x];

				u |= (table[5][x] << 1)&table[5][x];
				if (u > 0)
				{
					chainTable[x] |= u;
					chainTable[x] |= (u >> 1);
				}
			}
		}

		for (int x = 0; x < Width - 1; x++)
		{
			if ((recalc&RecalcBitMask[x]) > 0)
			{
				//右上
				uint32_t ru = 0;
				//右
				uint32_t r = 0;
				//右下
				uint32_t rd = 0;

				r |= (table[1][x])&table[9][x + 1];
				ru |= (table[1][x] << 1)&table[9][x + 1];
				rd |= (table[1][x] >> 1)&table[9][x + 1];

				r |= (table[9][x])&table[1][x + 1];
				ru |= (table[9][x] << 1)&table[1][x + 1];
				rd |= (table[9][x] >> 1)&table[1][x + 1];

				r |= (table[2][x])&table[8][x + 1];
				ru |= (table[2][x] << 1)&table[8][x + 1];
				rd |= (table[2][x] >> 1)&table[8][x + 1];

				r |= (table[8][x])&table[2][x + 1];
				ru |= (table[8][x] << 1)&table[2][x + 1];
				rd |= (table[8][x] >> 1)&table[2][x + 1];

				r |= (table[3][x])&table[7][x + 1];
				ru |= (table[3][x] << 1)&table[7][x + 1];
				rd |= (table[3][x] >> 1)&table[7][x + 1];

				r |= (table[7][x])&table[3][x + 1];
				ru |= (table[7][x] << 1)&table[3][x + 1];
				rd |= (table[7][x] >> 1)&table[3][x + 1];

				r |= (table[4][x])&table[6][x + 1];
				ru |= (table[4][x] << 1)&table[6][x + 1];
				rd |= (table[4][x] >> 1)&table[6][x + 1];

				r |= (table[6][x])&table[4][x + 1];
				ru |= (table[6][x] << 1)&table[4][x + 1];
				rd |= (table[6][x] >> 1)&table[4][x + 1];

				r |= (table[5][x])&table[5][x + 1];
				ru |= (table[5][x] << 1)&table[5][x + 1];
				rd |= (table[5][x] >> 1)&table[5][x + 1];

				if (ru > 0)
				{
					chainTable[x + 1] |= ru;
					chainTable[x + 0] |= (ru >> 1);
				}
				if (r > 0)
				{
					chainTable[x + 1] |= r;
					chainTable[x + 0] |= r;
				}
				if (rd > 0)
				{
					chainTable[x + 1] |= rd;
					chainTable[x + 0] |= (rd << 1);
				}
			}
		}

		uint16_t nextClac = 0;

		for (int x = 0; x < Width; x++)
		{
			if (chainTable[x] > 0)
			{
				nextClac |= RecalcBitMask[x];

				table[0][x] &= (~chainTable[x]);
			}
		}
		recalc = nextClac;

		return nextClac > 0;
	}

	const Chain bombBlock(uint16_t& recalc) {

		const Num BombNumber = 5;

		std::array<uint32_t, Width> bombTable = table[BombNumber];

		for (int x = 0; x < Width; x++)
		{
			bombTable[x] |= (table[BombNumber][x] << 1) & table[0][x];//上
		}
		for (int x = 0; x < Width - 1; x++)
		{
			bombTable[x + 1] |= (table[BombNumber][x] << 1) & table[0][x + 1];//右上
			bombTable[x + 1] |= (table[BombNumber][x]) & table[0][x + 1];//右
			bombTable[x + 1] |= (table[BombNumber][x] >> 1) & table[0][x + 1];//右下
		}

		recalc = 0;
		int disBlock = 0;
		for (int x = 0; x < Width; x++)
		{
			if (bombTable[x] > 0)
			{
				recalc |= RecalcBitMask[x];

				table[0][x] &= (~bombTable[x]);

				disBlock += popcnt(bombTable[x]);
			}
		}

		int score = 0;
		if (disBlock != 0)
			score = static_cast<int>(25.0 * pow(2.0, disBlock / 12.0));

		return Chain(0, score, score / 2);
	}

	const Chain chainBlock(uint16_t& recalc) {

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

	//table[0]で0になっているものを詰める
	void fallBlock(uint16_t& recalc) {

		uint16_t nextClac = 0;

		for (int x = 0; x < Width; x++)
		{
			if ((recalc & RecalcBitMask[x]) > 0)
			{
				//各数字を落下させる
				table[1][x] = pext(table[1][x], table[0][x]);
				table[2][x] = pext(table[2][x], table[0][x]);
				table[3][x] = pext(table[3][x], table[0][x]);
				table[4][x] = pext(table[4][x], table[0][x]);
				table[5][x] = pext(table[5][x], table[0][x]);
				table[6][x] = pext(table[6][x], table[0][x]);
				table[7][x] = pext(table[7][x], table[0][x]);
				table[8][x] = pext(table[8][x], table[0][x]);
				table[9][x] = pext(table[9][x], table[0][x]);
				table[11][x] = pext(table[11][x], table[0][x]);

				//ブロックを落下させる
				table[0][x] = pext(table[0][x], table[0][x]);

				nextClac |= RecalcLine[x];

				setElevation(x);
			}
		}

		recalc = nextClac;
	}

	void setPack(const Pack& pack, const Command& command, uint16_t& recalc) {

		const auto& pos = command.pos;
		const auto& r = command.rotate;

		recalc = RecalcLine[pos] | RecalcLine[pos + 1];

		if (pack[r][1][0] != Empty)
		{
			table[0][pos + 0] |= LineBit[elevation[pos + 0]];
			table[pack[r][1][0]][pos + 0] |= LineBit[elevation[pos + 0]];
			elevation[pos + 0]++;
		}
		if (pack[r][0][0] != Empty)
		{
			table[0][pos + 0] |= LineBit[elevation[pos + 0]];
			table[pack[r][0][0]][pos + 0] |= LineBit[elevation[pos + 0]];
			elevation[pos + 0]++;
		}

		if (pack[r][1][1] != Empty)
		{
			table[0][pos + 1] |= LineBit[elevation[pos + 1]];
			table[pack[r][1][1]][pos + 1] |= LineBit[elevation[pos + 1]];
			elevation[pos + 1]++;
		}
		if (pack[r][0][1] != Empty)
		{
			table[0][pos + 1] |= LineBit[elevation[pos + 1]];
			table[pack[r][0][1]][pos + 1] |= LineBit[elevation[pos + 1]];
			elevation[pos + 1]++;
		}

	}

public:

	Field() {
		for (auto& n : table) n.fill(0);
		elevation.fill(0);
	}

	[[nodiscard]]
	Field copy() const { return std::move(Field(*this)); }

	/// <summary>パックのドロップ</summary>
	/// <returns>パックドロップ時の結果</returns>
	[[nodiscard]]
	const Chain dropPack(const Pack& pack, const Command& command) {

		uint16_t recalc(false);

		setPack(pack, command, recalc);

		const auto chain = chainBlock(recalc);

		return chain;
	}

	/// <summary>スキルの使用</summary>
	/// <returns>スキル使用時の結果</returns>
	[[nodiscard]]
	const Chain useSkill() {

		uint16_t recalc(false);

		const auto bomb = bombBlock(recalc);

		fallBlock(recalc);

		const auto chain = chainBlock(recalc);

		return bomb + chain;
	}

	/// <summary>仮ブロック設置</summary>
	/// <returns>仮ブロック設置時の結果</returns>
	const Chain dropCell(const int pos, const int num) {

		uint16_t recalc(false);

		recalc = RecalcLine[pos];

		table[0][pos] |= LineBit[elevation[pos]];
		table[num][pos] |= LineBit[elevation[pos]];

		elevation[pos]++;

		const auto chain = chainBlock(recalc);

		return chain;
	}

	void dropGarbage() {

		for (int x = 0; x < Width; x++)
		{
			table[0][x] |= LineBit[elevation[x]];
			table[Garbage][x] |= LineBit[elevation[x]];
			elevation[x]++;
		}

	}

	[[nodiscard]]
	const bool isSurvival() const {
		return !std::any_of(elevation.cbegin(), elevation.cend(), [](int y) {
			return y >= Height - DangerLine;
		});
	}

	[[nodiscard]]
	const HashBit hash() const {

		HashBit code = 0;

		constexpr HashBit FNV_OFFSET_BASIS_32 = 2166136261U;
		constexpr HashBit FNV_PRIME_32 = 16777619U;

		for (int n = 0; n < 12; n++)
		{
			for (int x = 0; x < Width; x++)
			{
				code = (FNV_PRIME_32*code) ^ (table[n][x]);
			}
		}

		return code;
	}

	const int countBlock() const {

		int count = 0;

		count += popcnt(table[0][0]);
		count += popcnt(table[0][1]);
		count += popcnt(table[0][2]);
		count += popcnt(table[0][3]);
		count += popcnt(table[0][4]);
		count += popcnt(table[0][5]);
		count += popcnt(table[0][6]);
		count += popcnt(table[0][7]);
		count += popcnt(table[0][8]);
		count += popcnt(table[0][9]);

		count -= popcnt(table[11][0]);
		count -= popcnt(table[11][1]);
		count -= popcnt(table[11][2]);
		count -= popcnt(table[11][3]);
		count -= popcnt(table[11][4]);
		count -= popcnt(table[11][5]);
		count -= popcnt(table[11][6]);
		count -= popcnt(table[11][7]);
		count -= popcnt(table[11][8]);
		count -= popcnt(table[11][9]);

		return count;
	}

	//桂馬型のカウント
	const int formCheck() const {

		int count = 0;

		for (int x = 0; x < Width - 1; x++)
		{
			count += popcnt((table[1][x] << 2) & table[9][x + 1]);
			count += popcnt((table[1][x] >> 2) & table[9][x + 1]);

			count += popcnt((table[2][x] << 2) & table[8][x + 1]);
			count += popcnt((table[2][x] >> 2) & table[8][x + 1]);

			count += popcnt((table[3][x] << 2) & table[7][x + 1]);
			count += popcnt((table[3][x] >> 2) & table[7][x + 1]);

			count += popcnt((table[4][x] << 2) & table[6][x + 1]);
			count += popcnt((table[4][x] >> 2) & table[6][x + 1]);

			count += popcnt((table[5][x] << 2) & table[5][x + 1]);
			count += popcnt((table[5][x] >> 2) & table[5][x + 1]);

			count += popcnt((table[6][x] << 2) & table[4][x + 1]);
			count += popcnt((table[6][x] >> 2) & table[4][x + 1]);

			count += popcnt((table[7][x] << 2) & table[3][x + 1]);
			count += popcnt((table[7][x] >> 2) & table[3][x + 1]);

			count += popcnt((table[8][x] << 2) & table[2][x + 1]);
			count += popcnt((table[8][x] >> 2) & table[2][x + 1]);

			count += popcnt((table[9][x] << 2) & table[1][x + 1]);
			count += popcnt((table[9][x] >> 2) & table[1][x + 1]);

		}

		return count;
	}

	//[[deprecated("used for debug only")]]
	void debug() const {

		for (int y = Height - 1; y >= 0; y--)
		{
			for (int x = 0; x < Width; x++)
			{
				if ((table[0][x] & LineBit[y]) > 0)
				{
					for (int n = 1; n < 12; n++)
					{
						if ((table[n][x] & LineBit[y]) > 0)
						{
							std::cerr << n << " ";
						}
					}
				}
				else
				{
					std::cerr << "0 ";
				}
			}
			std::cerr << std::endl;
		}

	}

	[[nodiscard]]
	static Field Creat() {

		Field field;

		for (int y = 0; y < Height - DangerLine; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				int num;
				std::cin >> num;

				if (num > 0)
				{
					field.table[num][x] |= LineBit[(Height - DangerLine - 1) - y];
					field.table[0][x] |= LineBit[(Height - DangerLine - 1) - y];
				}
			}
			std::cin.ignore();
		}

		field.setElevation();

		return std::move(field);
	}

};

