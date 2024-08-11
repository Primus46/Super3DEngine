#include "Graphics/SGraphicsEngine.h"
#include "Graphics/SModel.h"
#include "Graphics/SShaderProgram.h"
#include "Math/SSTTransform.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTCamera.h"
#include "Graphics/SSTLight.h"


// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


// test for debug
TWeak<SModel> m_model;
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

	// create the camera
	m_camera = TMakeShared<SSTCamera>();
	m_camera->transform.position.z -= 25.0f;

	// creates the texture object
	TShared<STexture> defaultTexture = TMakeShared<STexture>();

	// add the texture to the mesh if it successfully created
	if (!defaultTexture->LoadTexture("Default Grid", "Textures/Name_1m x 1m.png")) {
		SDebug::Log("Graphics engine default texture failed to load", ST_ERROR);
	}


	// DEBUG
	m_model = ImportModel("Models/Helmet3/Helmet3.fbx");
	
	// creating a texture
	TShared<STexture> tex = TMakeShared<STexture>();
	tex->LoadTexture("head base colour", "Models/Helmet3/Textures/Head_Base_color.png");

	// creating a specular texture
	TShared<STexture> specTex = TMakeShared<STexture>();
	specTex->LoadTexture("head spec colour", "Models/Helmet3/Textures/Head_Specular.png");

	// creating a second texture
	TShared<STexture> tex2 = TMakeShared<STexture>();
	tex2->LoadTexture("face texture base colour", "Models/Helmet3/Textures/facetexture_Base_color.png");

	// creating a second specular texture
	TShared<STexture> specTex2 = TMakeShared<STexture>();
	specTex2->LoadTexture("face texture spec colour", "Models/Helmet3/Textures/facetexture_Specular.png");

	// creating a material
	TShared<SSTMaterial> mat = TMakeShared<SSTMaterial>();
	TShared<SSTMaterial> mat2 = TMakeShared<SSTMaterial>();
	// assigning the texture to the base colour map for the material
	mat->m_baseColourMap = tex;
	mat->m_specularMap = specTex;

	mat2->m_baseColourMap = tex2;
	mat2->m_specularMap = specTex2;

	// setting the material to the 0 slot in the model
	m_model.lock()->SetMaterialBySlot(0, mat);
	m_model.lock()->SetMaterialBySlot(1, mat2);


	// create the dir light
	const auto& dirLight = CreateDirLight();

	// check it exists as a reference
	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(0.0f, 0.0f, 0.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.1f);
	}

	/*m_pointLight = CreatePointLight();
	m_pointLight.lock()->position.z -= 10.0f;
	m_pointLight.lock()->colour = glm::vec3(1.0f, 0.0f, 0.5f);

	m_pointLight = CreatePointLight();
	m_pointLight.lock()->position.z += 10.0f;
	m_pointLight.lock()->colour = glm::vec3(0.0f, 1.0f, 0.5f);

	m_pointLight = CreatePointLight();
	m_pointLight.lock()->position.x -= 10.0f;
	m_pointLight.lock()->colour = glm::vec3(0.0f, 1.0f, 1.0f);

	m_pointLight = CreatePointLight();
	m_pointLight.lock()->position.x += 10.0f;
	m_pointLight.lock()->colour = glm::vec3(1.0f, 1.0f, 0.0f);*/

	// log the success of the graphics engine init
	SDebug::Log("Successfully initialised graphics engine", ST_SUCCESS);

	return true;
}

void SGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// there is a wireframe shader by default, this activates it
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// set background colour
	//glClearColor(0.0f, 1.0f, 0.5f, 1.0f);	// cool seafoam green-ish colour
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	// clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// m_model->GetTransform().position.x = -2.0f;

	m_model.lock()->GetTransform().rotation.x += 0.01f;
	m_model.lock()->GetTransform().rotation.y += 0.01f;
	m_model.lock()->GetTransform().rotation.z += 0.01f;

	//m_pointLight.lock()->position.z += 0.0005f;


	// activate the shader
	m_shader->Activate();

	// set the world transformations based on the camera
	m_shader->SetWorldTransform(m_camera);

	// rendered custom graphics
	// models will update their own positions in the mesh based on the transform
	for (const auto& modelRef : m_models) {
		modelRef->Render(m_shader, m_lights);
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

TWeak<SModel> SGraphicsEngine::ImportModel(const SString& path)
{
	const auto& newModel = TMakeShared<SModel>();
	newModel->ImportModel(path);
	m_models.push_back(newModel);

	return newModel;
}

TShared<SSTMaterial> SGraphicsEngine::CreateMaterial()
{
	return TMakeShared<SSTMaterial>();
}
