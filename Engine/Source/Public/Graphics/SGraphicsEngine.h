#pragma once
#include "EngineTypes.h"
#include "Graphics/SSTMaterial.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class SModel;
class SShaderProgram;
struct SSTCamera;
struct SSTLight;
struct SSTPointLight;
struct SSTDirLight;

class SGraphicsEngine {
public:
	SGraphicsEngine() = default;
	~SGraphicsEngine() = default;

	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// render the gaphics engine
	void Render(SDL_Window* sdlWindow);

	// retur a weak versipn of the camera
	TWeak<SSTCamera> GetCamera() { return m_camera; }

	// create a point light and return a weak pointer
	TWeak<SSTPointLight> CreatePointLight();

	// create a dir light and return a weak pointer
	TWeak<SSTDirLight> CreateDirLight();

	// Import a model and return a weak pointer
	TShared<SModel> ImportModel(const SString& path);

	// create a material for the engine
	TShared<SSTMaterial> CreateMaterial();

private:
	// storing memoery location for open gl context
	SDL_GLContext m_sdlGLContext;

	// store the shader for the engine
	TShared<SShaderProgram> m_shader;

	// store the wireframe shader
	TShared<SShaderProgram> m_wireShader;

	// store the camera
	TShared<SSTCamera> m_camera;

	// stores all lights in the engine
	TArray<TShared<SSTLight>> m_lights;

	// store all models in the engine
	TArray<TWeak<SModel>> m_models;

	// default material for all models
	TShared<SSTMaterial> m_defaultMaterial;
};