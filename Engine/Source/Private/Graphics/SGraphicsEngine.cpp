#include "Graphics/SGraphicsEngine.h"
#include "Graphics/SModel.h"
#include "Graphics/SShaderProgram.h"
#include "Math/SSTTransform.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTCamera.h"


// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


// test for debug
TUnique<SModel> m_model;
TUnique<SModel> m_model2;

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

	// enable depth to be tested
	glEnable(GL_DEPTH_TEST);

	// create the shader object
	m_shader = TMakeShared<SShaderProgram>();

	// attempt to init shader and test if failed
	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"
	)) {
		SDebug::Log("Graphics engine failed to initialise due to shader failure");
		return false;
	}

	// create the camera
	m_camera = TMakeShared<SSTCamera>();
	m_camera->transform.position.z -= 5.0f;

	// creates the texture object
	TShared<STexture> defaultTexture = TMakeShared<STexture>();

	// add the texture to the mesh if it successfully created
	if (!defaultTexture->LoadTexture("Default Grid", "Textures/Name_1m x 1m.png")) {
		SDebug::Log("Graphics engine default texture failed to load", ST_ERROR);
	}

	// DEBUG
	m_model = TMakeUnique<SModel>();
	m_model->MakeCube(defaultTexture);
	m_model2 = TMakeUnique<SModel>();
	m_model2->MakePyramid(defaultTexture);



	// log the success of the graphics engine init
	SDebug::Log("Successfully initialised graphics engine", ST_SUCCESS);

	return true;
}

void SGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// there is a wireframe shader by default, this activates it
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// set background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// m_model->GetTransform().position.x = -2.0f;

	// m_model->GetTransform().rotation.x += 0.01f;
	m_model->GetTransform().rotation.y += 0.01f;
	//m_model->GetTransform().rotation.z += 0.01f;

	// activate the shader
	m_shader->Activate();

	// set the world transformations based on the camera
	m_shader->SetWorldTransform(m_camera);

	// rendered custom graphics
	// models will update their own positions in the mesh based on the transform
	m_model->Render(m_shader);

	// m_model2->GetTransform().position.x = 2.0f;

	// m_model2->GetTransform().rotation.y -= 0.01f;

	// m_model2->Render(m_shader);

	// presented the frame to the window
	// swaping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);

	//SDebug::Log("render");

}
