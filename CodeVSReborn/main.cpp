#include "Base.hpp"

#include "Ai.hpp"

std::shared_ptr<Share> Share::instance;
std::shared_ptr<Random> Random::instance;

void simulationDebug(const Command& com) {

	const auto share = Share::Get();
	const auto pack = share->pack();

	auto next = share->my().copy();
	const auto c = next.simulation(com, pack);

	std::cerr << "Simulation Debug" << std::endl;
	next.debug();
	c.debug();
	std::cerr << com.toString() << std::endl;
	next.field.debug();
}

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
		const auto command = ai.think();
		sw.stop();

		simulationDebug(command);

		std::cerr << sw.toString_ms() << std::endl;
		std::cerr.flush();

		std::cout << command.toString() << std::endl;
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

		const auto c = test.simulation(Command(true), Pack());

	}
	sw.stop();

	std::cerr << sw.toString_ms() << std::endl;

}

void timerTest() {

	Stopwatch sw;
	MilliSecTimer mTimer(std::chrono::milliseconds(1000));

	sw.start();
	mTimer.start();
	while (!mTimer) {}
	sw.stop();
	std::cerr << sw.toString_us() << std::endl;
	std::cerr << sw.toString_ms() << std::endl;


}

int main() {

	Share::Create();
	Random::Create();

	std::cout << "ao1415" << std::endl;
	std::cout.flush();

	run();
	//test();
	//timerTest();

	return 0;
}
