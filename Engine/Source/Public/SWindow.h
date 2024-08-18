#pragma once
#include "EngineTypes.h"
#include "Math/SSTTransform.h"

class SGraphicsEngine;
class SInput;

struct SSTWindowParams {
	SSTWindowParams() {
		title = "Super 3D Engine Window";
		x = y = 0;
		w = 1280;
		h = 720;
		vsync = false;
		fullscreen = false;
	}

	SSTWindowParams(std::string title, int x, int y, 
		unsigned int w, unsigned int h) : 
		title(title), 
		x(x), y(y), 
		w(w), h(h), 
		vsync(false), 
		fullscreen(false){}

	// title of the window
	std::string title;
	// position of the window
	int x, y;
	// width and height of the window
	unsigned int w, h;
	// vsync; enabled
	bool vsync;
	// fullscreen enable
	bool fullscreen;

};

struct SDL_Window;


class SWindow {
public:
	SWindow();
	~SWindow();

	// create the window with parameters
	bool CreateWindow(const SSTWindowParams& params);

	// close the window
	void CloseWindow() { m_shouldClose = true; }

	// close if the window has been set to close
	bool IsPendingClose() { return m_shouldClose; }

	void RegisterInput(const TShared<SInput>& m_input);

	// render the graphics engine
	void Render();

	// get the graphics engine
	TUnique<SGraphicsEngine>& GetGraphics() { return m_graphicsEngine; }

private:
	// a ref to the window in sdl
	SDL_Window* m_sdlWindow;

	// window parameters
	SSTWindowParams m_params;

	// determine if the window shoud close
	bool m_shouldClose;

	// store the graphics engine
	TUnique<SGraphicsEngine> m_graphicsEngine;

	// direction to move the camera
	glm::vec3 m_cameraDirection;

	// amount to rotate the camera
	glm::vec3 m_cameraRotation;

	// can zoom
	bool m_canZoom;

	// is the user in input mode
	bool m_inputMode;
};
