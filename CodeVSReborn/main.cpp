#include "Base.hpp"

#include "Ai.hpp"

std::shared_ptr<Share> Share::instance;
std::shared_ptr<Random> Random::instance;

void run() {

	auto share = Share::Get();
	share->first();

	Stopwatch sw;
	Ai ai;

	for (int i = 0; i < MaxTurn; i++)
	{
		if (!share->loop())
			break;

		std::cerr << "turn:" << std::setw(3) << share->turn() << "================" << std::endl;
		sw.start();
		const std::string command = ai.think();
		sw.stop();

		std::cerr << sw.toString_ms() << std::endl;
		std::cerr.flush();

		std::cout << command << std::endl;
		std::cout.flush();
	}

}

void test() {

	PlayerInfo base;
	base.field = Field::Creat();

	Stopwatch sw;

	sw.start();
	for (int i = 0; i < 100000; i++)
	{
		auto test = base.copy();

		test.simulation(Command(true), Pack());

	}
	sw.stop();

	std::cerr << sw.toString_ms() << std::endl;

}

int main() {

	Share::Create();
	Random::Create();

	std::cout << "ao1415" << std::endl;
	std::cout.flush();

	//run();
	test();

	return 0;
}
