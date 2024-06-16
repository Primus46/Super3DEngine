#include "SWindow.h"
#include "Graphics/SGraphicsEngine.h"
#include "Debugging/SDebug.h"

// External Libs
#include <SDL/SDL.h>

SWindow::SWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;

	std::cout << "Window created." << std::endl;
}

SWindow::~SWindow()
{
	// if sdl window exists, destroy it
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	std::cout << "Window destroyed." << std::endl;
}

bool SWindow::CreateWindow(const SSTWindowParams& params)
{
	// enabling opengl in the sdl window
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	// assigning paramaters to the member for the window
	m_params = params;

	// add vsync flag if it's selected
	if (m_params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	// set fullscreen if it's set
	if (m_params.fullscreen)
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP; // Fullscreen borderless
	else
		windowFlags += SDL_WINDOW_SHOWN; // windowed mode

	m_sdlWindow = SDL_CreateWindow(
		m_params.title.c_str(),
		m_params.x,
		m_params.y,
		m_params.w,
		m_params.h,
		windowFlags
	);

	// check if sdl errors are created 
	if (!m_sdlWindow) {
		std::cout << "SDL Failed to create window: " << SDL_GetError() << std::endl;
		CloseWindow();
		return false;
	}

	// creates the graphics engine object
	m_graphicsEngine = std::make_unique<SGraphicsEngine>();

	// initialises the graphics engine and checks
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		SDebug::Log("Window failed to initialised graphics engine", ST_ERROR);
		m_graphicsEngine = nullptr;
		return false;
	}

	return true;
}

void SWindow::Render()
{	
	// render the graphics engine if one exists
	if (m_graphicsEngine) {
		m_graphicsEngine->Render(m_sdlWindow);
	}
}
