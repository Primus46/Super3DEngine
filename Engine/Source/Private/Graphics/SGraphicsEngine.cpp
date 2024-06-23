#include "Graphics/SGraphicsEngine.h"
#include "Debugging/SDebug.h"
#include "Graphics/SMesh.h"
#include "Graphics/SShaderProgram.h"
#include "Math/SSTTransform.h"
#include "Graphics/STexture.h"


// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

std::vector<SSTVertexData> vertexData = {
	//	 x	   y	 z        r     g     b
	// square
	{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// vertx data 1 - top left
	{ { 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // vertx data 2 - top right
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // vertx data 3 - bottom left
	{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // vertx data 4 - bottom right

	// triangle
	//{ {-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }, // vertx data 1 - top left
	//{ {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }, // vertx data 2 - top right
	//{ { 0.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} } // vertx data 3 - bottom 
};

std::vector<uint32_t> indexData = {
	0, 1, 2, // triangle 1
	1, 2, 3, // triangle 2

	//4, 5, 6  // triangle 3
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

	// creates the texture object
	TShared<STexture> defaultTexture = TMakeShared<STexture>();

	// add the texture to the mesh if it successfully created
	if (defaultTexture->LoadTexture("Default Grid", "Textures/Name_1m x 1m.png")) {
		m_mesh->SetTexture(defaultTexture);
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

	static SSTTransform SquareTransform;
	SquareTransform.position.x = -0.2f;
	SquareTransform.rotation.z = 45.0f; // monitor refresh rate is 165hz so this is as fast as I  can rotate it
	SquareTransform.scale = glm::vec3(1.0f);

	// rendered custom graphics
	m_mesh->Render(m_shader, SquareTransform);

	static SSTTransform TriangleTransform; // the rotation variable was making the second mesh stay on it's side even when the other 2 variable changed fine so made a second variable to avoid conflicts

	TriangleTransform.position.x = 0.8f;
	TriangleTransform.rotation.z = 0.0f;
	TriangleTransform.scale = glm::vec3(0.25f);
	m_mesh->Render(m_shader, TriangleTransform);

	// presented the frame to the window
	// swaping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);

	//SDebug::Log("render");

}
