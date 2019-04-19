#include "Base.hpp"

#include "Ai.hpp"

std::shared_ptr<Share> Share::instance;

int main() {

	Share::Create();
	auto share = Share::Get();


	std::cout << "ao1415" << std::endl;
	std::cout.flush();

	share->first();

	Stopwatch sw;
	Ai ai;

	for (int i = 0; i < MaxTurn; i++)
	{
		share->loop();

		std::cerr << "turn:" << std::setw(3) << share->turn() << "================" << std::endl;
		sw.start();
		const std::string command = ai.think();
		sw.stop();

		std::cerr << sw.toString_ms() << std::endl;
		std::cerr.flush();

		std::cout << command << std::endl;
		std::cout.flush();
	}


	return 0;
}
