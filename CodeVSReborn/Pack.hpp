#pragma once

#include "Base.hpp"

class Pack {
private:

	std::array<PackArray, 4> packs;

	PackArray rotate(const PackArray& basePack) {

		PackArray rotatedPack;

		rotatedPack[0][0] = basePack[1][0];
		rotatedPack[0][1] = basePack[0][0];
		rotatedPack[1][0] = basePack[1][1];
		rotatedPack[1][1] = basePack[0][1];

		return std::move(rotatedPack);
	}

	PackArray& operator[](const int n) { return this->packs[n]; }

public:

	const PackArray& operator[](const int n) const { return this->packs[n]; }

	static Pack&& Create() {

		Pack pack;

		PackArray basePack;
		std::string end;

		std::cin >> basePack[0][0] >> basePack[0][1];
		std::cin >> basePack[1][0] >> basePack[1][1];

		std::cin >> end;

		pack.packs[0] = basePack;
		pack.packs[1] = pack.rotate(pack.packs[0]);
		pack.packs[2] = pack.rotate(pack.packs[1]);
		pack.packs[3] = pack.rotate(pack.packs[2]);

		return std::move(pack);
	}

};
