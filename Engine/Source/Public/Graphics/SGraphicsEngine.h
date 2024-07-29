#pragma once
#include "EngineTypes.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class SMesh;
class SShaderProgram;
struct SSTCamera;
struct SSTLight;

class SGraphicsEngine {
public:
	SGraphicsEngine() = default;
	~SGraphicsEngine() = default;

	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// render the gaphics engine
	void Render(SDL_Window* sdlWindow);

	// retur a weak versipn of the camera
	TWeak<SSTCamera> GetCamera() { return m_camera; }


private:
	// storing memoery location for open gl context
	SDL_GLContext m_sdlGLContext;

	// store the shader for the engine
	TShared<SShaderProgram> m_shader;

	// store the camera
	TShared<SSTCamera> m_camera;

	// stores all lights in the engine
	TArray<TShared<SSTLight>> m_lights;
};