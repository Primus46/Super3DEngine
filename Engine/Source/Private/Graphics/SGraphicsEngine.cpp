#include "Graphics/SGraphicsEngine.h"
#include "Graphics/SMesh.h"
#include "Graphics/SModel.h"
#include "Graphics/SShaderProgram.h"
#include "Math/SSTTransform.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTCamera.h"
#include "Graphics/SSTLight.h"
#include "Math/SSTCollision.h"


// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

// collision cube vertices
const std::vector<SSTVertexData> colMeshVData = {
	// x   // y   // z 
{ {	-1.0f, -1.0f,  1.0f } }, // bl f
{ {	 1.0f, -1.0f,  1.0f } }, // br f
{ {  1.0f,  1.0f,  1.0f } }, // tr f
{ { -1.0f,  1.0f,  1.0f } }, // tl f
{ {	-1.0f, -1.0f, -1.0f } }, // bl b
{ {	 1.0f, -1.0f, -1.0f } }, // br b
{ {  1.0f,  1.0f, -1.0f } }, // tr b
{ { -1.0f,  1.0f, -1.0f } }, // tl b
};

// collision cube indices
const std::vector<uint32_t> colMeshIData = {
	0, 1, 1, 2, 2, 3, 3, 0, // front
	4, 5, 5, 6, 6, 7, 7, 4, // back
	0, 4, 1, 5, 2, 6, 3, 7  // sides
};

// test for debug
TWeak<SSTPointLight> m_pointLight;

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
	
	m_wireShader = TMakeShared<SShaderProgram>();

	if (!m_wireShader->InitShader(
		"Shaders/Wireframe/Wireframe.vertex",
		"Shaders/Wireframe/Wireframe.frag"
	)) {
		SDebug::Log("Graphics engine failed to initialise due to wireframe shader failure");
		return false;
	}


	// create the camera
	m_camera = TMakeShared<SSTCamera>();
	m_camera->transform.position.z -= 25.0f;

	// creates the texture object
	TShared<STexture> defaultTexture = TMakeShared<STexture>();

	// add the texture to the mesh if it successfully created
	if (!defaultTexture->LoadTexture("Default Grey", "Textures/T_Default_Grey.png")) {
		SDebug::Log("Graphics engine default texture failed to load", ST_ERROR);
	}

	// init a default material for all models
	m_defaultMaterial = TMakeShared<SSTMaterial>();

	// set the texture for the default material
	m_defaultMaterial->m_baseColourMap = defaultTexture;
	m_defaultMaterial->specularStrength = 0.0f;


	// create the dir light
	const auto& dirLight = CreateDirLight();

	// check it exists as a reference
	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(0.0f, 0.0f, 0.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.4f);
		lightRef->intensity = 5.0f;
	}

	const auto& pointLight = CreatePointLight();
	const auto& pointLight2 = CreatePointLight();
	const auto& pointLight3 = CreatePointLight();
	const auto& pointLight4 = CreatePointLight();

	if (const auto& lightRef = pointLight.lock()) {
		lightRef->position = glm::vec3(30.0f, 15.0f, 150.0f);
		lightRef->colour = glm::vec3(1.0f, 0.0f, 0.5f);
		lightRef->intensity = 5.0f;
	}

	if (const auto& lightRef = pointLight2.lock()) {
		lightRef->position = glm::vec3(10.0f, -60.0f, 0.0f);
		lightRef->colour = glm::vec3(0.0f, 1.0f, 0.5f);
		lightRef->intensity = 5.0f;
	}

	if (const auto& lightRef = pointLight3.lock()) {
		lightRef->position = glm::vec3(100.0f, 15.0f, 0.0f);
		lightRef->colour = glm::vec3(0.0f, 0.5f, 1.0f);
		lightRef->intensity = 5.0f;
	}

	if (const auto& lightRef = pointLight4.lock()) {
		lightRef->position = glm::vec3(-50.0f, 25.0f, -75.0f);
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.0f);
		lightRef->intensity = 5.0f;
	}

	// log the success of the graphics engine init
	SDebug::Log("Successfully initialised graphics engine", ST_SUCCESS);

	return true;
}

void SGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// there is a wireframe shader by default, this activates it
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// set background colour
	//glClearColor(0.0f, 1.0f, 0.5f, 1.0f); // cool seafoam green-ish colour // Debug testing
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// black background
	// clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//m_pointLight.lock()->position.z += 0.0005f;


	// activate the shader
	m_shader->Activate();

	// set the world transformations based on the camera
	m_shader->SetWorldTransform(m_camera);

	// rendered custom graphics
	// models will update their own positions in the mesh based on the transform
	for (int i = m_models.size() - 1; i >= 0; --i) {
		// detecting if the ereference exists
		if (const auto& modelRef = m_models[i].lock()) {
			// render if thre is a reference
			modelRef->Render(m_shader, m_lights);
		}
		else {
			// erase from the array if there is no reference
			m_models.erase(m_models.begin() + i);
		}

	}

	//__________Wireframe Shader__________
	if (m_collisions.size() > 0)
	{
		// activate the shader
		m_wireShader->Activate();

		// set the world transformations based on the camera
		m_wireShader->SetWorldTransform(m_camera);

		// rendered custom graphics
		// models will update their own positions in the mesh based on the transform
		for (int i = m_collisions.size() - 1; i >= 0; --i) {
			// detecting if the ereference exists
			if (const auto& colRef = m_collisions[i].lock()) {
				// convert position of collision into transform
				SSTTransform transform;
				transform.position = colRef->box.position;
				transform.scale = colRef->box.halfSize;

				// set the colour of the box
				m_wireShader->SetWireColour(colRef->debugColour);

				// render if thre is a reference
				colRef->debugMesh->WireRender(m_wireShader, transform);
			}
			else {
				// erase from the array if there is no reference
				m_collisions.erase(m_collisions.begin() + i);
			}

		}
	}

	// presented the frame to the window
	// swaping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);

	//SDebug::Log("render");

}

TWeak<SSTPointLight> SGraphicsEngine::CreatePointLight()
{
	const auto& newLight = TMakeShared<SSTPointLight>();
	m_lights.push_back(newLight);

	return newLight;
}

TWeak<SSTDirLight> SGraphicsEngine::CreateDirLight()
{
	const auto& newLight = TMakeShared<SSTDirLight>();
	m_lights.push_back(newLight);

	return newLight;
}

TShared<SModel> SGraphicsEngine::ImportModel(const SString& path)
{
	const auto& newModel = TMakeShared<SModel>();
	newModel->ImportModel(path, m_defaultMaterial);
	m_models.push_back(newModel);

	return newModel;
}

TShared<SSTMaterial> SGraphicsEngine::CreateMaterial()
{
	return TMakeShared<SSTMaterial>();
}

void SGraphicsEngine::CreateCollisionMesh(const TWeak<SSTCollision>& col)
{
	if (const auto& colRef = col.lock())
	{
		TShared<SMesh> newMesh = TMakeShared<SMesh>();

		// create a box of lines
		newMesh->CreateMesh(colMeshVData, colMeshIData);

		// stroe the shared mesh into the collision
		colRef->debugMesh = newMesh;

		// add the mesh to be rendered
		m_collisions.push_back(col);
	}
}
