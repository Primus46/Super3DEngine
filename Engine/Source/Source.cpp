// System Libs
#include <iostream>

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "SWindow.h"

// -- smart pointers delete themselves when there is no owner reference
// shared pointer = shares ownership across all references
// unique pointer = does not share ownership with anything else
// weak pointer = this has no ownership over any references

// source variables
std::unique_ptr<SWindow> m_window;

// source functions
bool Initialise()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cout << "Failed to Initalise SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// creating window object
	m_window = std::make_unique<SWindow>();

	// creating an sdl window
	if (!m_window->CreateWindow({ "Game window",
		SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		1280, 720 }))
		return false;

	return true;
}

void Cleanup() {
	// exit SDL and deallocate all initilised SDL components
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	// Failed to Initialise
	if (!Initialise()) {
		Cleanup();
		return -1;
	}

	// keep the game open as long as the window is open
	while (!m_window->IsPendingClose()) {
		// TO DO: Game Loop
		SDL_Event e;

		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT) {
				m_window->CloseWindow();
			}
		}
	}

	// Clean up engine return 0
	Cleanup();

	m_window = nullptr;

	std::cout << "Closing app" << std::endl;

	return 0;
}