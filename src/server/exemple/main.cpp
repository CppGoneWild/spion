#include "logg.hh"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>

#include <unistd.h>

#include "../spion.hh"

#include <list>



logg::Timer globaltimer;
logg::Timer rendertimer;
logg::Timer imguitimer;


void debug_timer_ctrl()
{
	if (ImGui::Begin("Profiling")) {
		if (ImGui::Button("reset timers")) {
			globaltimer.reset_stat();
			rendertimer.reset_stat();
			imguitimer.reset_stat();
		}
	}
	ImGui::End();
}


int main(int, char const * const *, char const * const *)
{
	// init log timer
	//globaltimer = COUT_TIMER("main game loop");
	//rendertimer = COUT_TIMER("rendering     ");
	//imguitimer  = COUT_TIMER("imgui         ");

	// sfml
	//sf::RenderWindow window;
	//window.create(sf::VideoMode(1900, 900), "spion");
	//window.setFramerateLimit(60);

	// imgui
	//ImGui::SFML::Init(window);


	// network


	spion::init(4242);

	COUT_INFO << "Main loop start";


	// main loop
	while (true ) {
/*
		logg::ScopeTimer st(globaltimer);

		{
			logg::ScopeTimer st(imguitimer);

			// event loop
			for (sf::Event sfevent; window.pollEvent(sfevent); ) {
				ImGui::SFML::ProcessEvent(sfevent);
				if (sfevent.type == sf::Event::Closed)
					window.close();
			}
			ImGui::SFML::Update(window, sf::milliseconds(160));


			debug_timer_ctrl();

		}
*/


		//ImGui::ShowDemoWindow();

		// render sfml
		//window.clear(sf::Color::Black);


		// render imgui
		//{
		//	logg::ScopeTimer st(imguitimer);
		// 	ImGui::SFML::Render(window);				
		//}


		// push to screen
		//window.display();
		//usleep(60);
	}

	COUT_INFO << "Main loop end";

	spion::clean();


	// clean imgui and sfml
	//ImGui::SFML::Shutdown();
	//window.close();

	return (0);
}

