#include "EngineTypes.h"

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "SWindow.h"
#include "Listeners/SInput.h"

// -- smart pointers delete themselves when there is no owner reference
// shared pointer = shares ownership across all references
// unique pointer = does not share ownership with anything else
// weak pointer = this has no ownership over any references

// source variables
TShared<SWindow> m_window;
TUnique<SInput> m_input;

// source functions
bool Initialise()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cout << "Failed to Initalise SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// tell SDL that we'll be rendering in gl version 460 or 4.60
	// 4 is the major version 
	// .60 is the minor version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// make sure that if open gl is using a version not in SDL we run SDL in compatibility mode
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	// set the bit depth for each colour
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);


	// creating window object
	m_window = TMakeShared<SWindow>();

	// creating an sdl window
	if (!m_window->CreateWindow({ "Game window",
		SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		720, 720 }))
		return false;

	// m_input = TMakeUnique<SInput>();
	// m_input->InitInput(m_window);

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

		// render the window
		m_window->Render();
	}

	// Clean up engine return 0
	Cleanup();

	m_window = nullptr;

	std::cout << "Closing app" << std::endl;

	return 0;
}