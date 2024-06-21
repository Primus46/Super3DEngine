#include "Graphics/SGraphicsEngine.h"
#include "Debugging/SDebug.h"
#include "Graphics/SMesh.h"
#include "Graphics/SShaderProgram.h"
#include "Math/SSTTransform.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

const std::vector<SSTVertexData> vertexData = {
	//	 x	   y	 z        r     g     b
	{ {-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} }, // vertx data 1 - top left
	{ {0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} }, // vertx data 2 - top right
	{ {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} }, // vertx data 3 - bottom left
	{ {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f} } // vertx data 4 - bottom right
};
const std::vector<uint32_t> indexData = {
	0, 1, 2, // triangle 1
	1, 2, 3 // triangle 2
};
// test for debug
std::unique_ptr<SMesh> m_mesh;

bool SGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr) {
		SDebug::Log("SDL window was null", ST_ERROR);
		SDebug::Log("graphics engine failed to initialise", ST_ERROR);
		return false;
	}

	// create an open gl context
	m_sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	// test if the context failed
	if (m_sdlGLContext == nullptr) {
		SDebug::Log("SDL failed to create gl context:" + std::string(SDL_GetError()), ST_ERROR);
		SDebug::Log("graphics engine failed to initialise", ST_ERROR);
		return false;
	}

	// make the current context active for the SDL window
	// test if it failed
	if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {
		SDebug::Log("SDL failed to make gl context current" + std::string(SDL_GetError()), ST_ERROR);
		SDebug::Log("graphics engine failed to initialise", ST_ERROR);
		return false;
	}

	if (vsync) {
		// try enable adaptive vsync and test if it failed
		if (SDL_GL_SetSwapInterval(-1) != 0) {
			// try to enable standard vsync and test if it failed
			if (SDL_GL_SetSwapInterval(-1) != 0) { 
				SDebug::Log(
					"graphics engine failed to initialise vsync: " + std::string(SDL_GetError()),
					ST_WARN);
				return false;
			}
		}
	}

	// initialise glew
	GLenum glewResults = glewInit();

	if (glewResults != GLEW_OK) {
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResults));
		SDebug::Log("Graphics engine dailed to initialise GLEW: " + errorMsg);
		return false;
	}

	m_shader = std::make_shared<SShaderProgram>();

	// attempt to init shader and test if failed
	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"
	)) {
		SDebug::Log("Graphics engine failed to initialise due to shader failure");
		return false;
	}

	// log the success of the graphics engine init
	SDebug::Log("Successfully initialised graphics engine", ST_SUCCESS);

	// create tne debug mesh
	m_mesh = std::make_unique<SMesh>();

	

	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		SDebug::Log("Failed to create mesh");
	}

	return true;
}

void SGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// there is a wireframe shader by default, this activates it
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// set background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	static SSTTransform transform;
	// transform.position.x = 0.0f;
	// transform.rotation.z += 0.01f;
	// transform.scale = glm::vec3(1.0f);

	// rendered custom graphics
	m_mesh->Render(m_shader, transform);
	

	// presented the frame to the window
	// swaping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);

	//SDebug::Log("render");

}
