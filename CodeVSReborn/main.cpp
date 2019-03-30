#include "Base.hpp"

#include "Ai.hpp"

std::shared_ptr<Share> Share::instance;

int main() {

	Share::Create();
	Stopwatch sw;

	std::cout << "ao1415" << std::endl;
	std::cout.flush();

	Input input;
	input.first();

	Ai ai;


	for (int i = 0; i < MaxTurn; i++)
	{
		input.loop();

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
