#include "logg.hh"

#include "../spion.hh"

#include <list>
#include <thread>


int main(int, char const * const *, char const * const *)
{
	spion::init(4242);

	COUT_INFO << "Main loop start";

	int i = 25;

	// main loop
	while (true ) {

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		spion::send_ro("test_2", 42);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		spion::send_ro("test_3", 42.42);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		spion::send_rw("test_w", i);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	COUT_INFO << "Main loop end";

	spion::clean();

	return (0);
}

