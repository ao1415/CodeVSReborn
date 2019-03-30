#pragma once

#include "Base.hpp"
#include "Pack.hpp"

class Field {
private:

	FieldArray table;

public:

	Field() {
		table.fill(0);
	}

	void setNextField(const Pack& pack, const Command& command) {

	}

	Field&& getNextField(const Pack& pack, const Command& command) {

		Field next;

		return std::move(next);
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

		return std::move(field);
	}

};

