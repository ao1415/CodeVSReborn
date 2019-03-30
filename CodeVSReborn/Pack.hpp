#pragma once

#include "Base.hpp"

class Pack {
private:

	std::array<PackArray, 4> packs;

	static PackArray&& Rotate(const PackArray& basePack) {

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

		basePack[0][0] = readNum();
		basePack[0][1] = readNum();
		basePack[1][0] = readNum();
		basePack[1][1] = readNum();

		std::cin >> end;

		pack[0] = basePack;
		pack[1] = Rotate(pack[0]);
		pack[2] = Rotate(pack[1]);
		pack[3] = Rotate(pack[2]);

		return std::move(pack);
	}

};
