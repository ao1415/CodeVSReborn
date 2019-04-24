#pragma once

#include "Base.hpp"

class Pack {
private:

	std::array<PackArray, 4> packs;

	[[nodiscard]]
	PackArray rotate(const PackArray& basePack) {

		PackArray rotatedPack;

		rotatedPack[0][0] = basePack[1][0];
		rotatedPack[0][1] = basePack[0][0];
		rotatedPack[1][0] = basePack[1][1];
		rotatedPack[1][1] = basePack[0][1];

		return std::move(rotatedPack);
	}

	[[nodiscard]]
	PackArray& operator[](const int n) { return this->packs[n]; }

public:

	[[nodiscard]]
	const PackArray& operator[](const int n) const { return this->packs[n]; }

	[[nodiscard]]
	static Pack Create() {

		Pack pack;

		PackArray basePack;
		std::string end;

		int p1, p2;
		int p3, p4;
		std::cin >> p1 >> p2;
		std::cin >> p3 >> p4;

		std::cin >> end;

		basePack[0][0] = static_cast<Num>(p1);
		basePack[0][1] = static_cast<Num>(p2);
		basePack[1][0] = static_cast<Num>(p3);
		basePack[1][1] = static_cast<Num>(p4);

		pack.packs[0] = basePack;
		pack.packs[1] = pack.rotate(pack.packs[0]);
		pack.packs[2] = pack.rotate(pack.packs[1]);
		pack.packs[3] = pack.rotate(pack.packs[2]);

		return std::move(pack);
	}

};
