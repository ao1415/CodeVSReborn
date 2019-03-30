#pragma once

#include "Base.hpp"
#include "Pack.hpp"
#include "Field.hpp"
#include "Share.hpp"

class Ai {
private:


public:

	std::string think() {

		Command com;
		com.pos = rand() % PackDropRange;
		com.rotate = rand() % 4;

		return com.toString();
	}

};
