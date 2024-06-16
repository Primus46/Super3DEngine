#pragma once
#include <iostream>

typedef void* SDL_GLContext;
struct SDL_Window;
class SMesh;
class SShaderProgram;

class SGraphicsEngine {
public:
	SGraphicsEngine() = default;
	~SGraphicsEngine() = default;

	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// render the gaphics engine
	void Render(SDL_Window* sdlWindow);

private:
	// storing memoery location for open gl context
	SDL_GLContext m_sdlGLContext;

	// store the shader for the engine
	std::shared_ptr<SShaderProgram> m_shader;
};