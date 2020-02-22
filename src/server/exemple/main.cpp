#include "logg.hh"

#include "../spion.hh"

#include <list>
#include <thread>


int main(int, char const * const *, char const * const *)
{
	spion::init(4242);

	COUT_INFO << "Main loop start";


	// main loop
	while (true ) {

		spion::send_ro("test");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}

	COUT_INFO << "Main loop end";

	spion::clean();

	return (0);
}

