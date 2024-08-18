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
	
	//// create the shader object
	//m_wireShader = TMakeShared<SShaderProgram>();

	//// attempt to init shader and test if failed
	//if (!m_wireShader->InitShader(
	//	"Shaders/Wireframe/Wireframe.vertex",
	//	"Shaders/Wireframe/Wireframe.frag"
	//)) {
	//	SDebug::Log("Graphics engine failed to initialise due to shader failure");
	//	return false;
	//}

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

	// DEBUG
	//ImportModel("Models/Gem/gem1.glb");
	/*m_model.lock()->GetTransform().position.x = 15.0f;
	m_model.lock()->GetTransform().scale = glm::vec3(0.75f);*/

	TShared<STexture> GemtTex1 = TMakeShared<STexture>();
	GemtTex1->LoadTexture("Gem base colour", "Models/Gem/Textures/gem1_EMISSIVE_0.jpeg");
	
	TShared<STexture> GemSpecTex1 = TMakeShared<STexture>();
	GemSpecTex1->LoadTexture("Gem spec colour", "Models/Gem/Textures/gem1_NORMAL_1.jpeg");
	
	TShared<STexture> GemtTex2 = TMakeShared<STexture>();
	GemtTex2->LoadTexture("Gem base colour", "Models/Gem/Textures/gem1_BASECOLOR_2.jpeg");

	TShared<STexture> GemSpecTex2 = TMakeShared<STexture>();
	GemSpecTex2->LoadTexture("Gem spec colour", "Models/Gem/Textures/gem1_Specular.png");
	
	TShared<SSTMaterial> mat3 = TMakeShared<SSTMaterial>();
	TShared<SSTMaterial> mat4 = TMakeShared<SSTMaterial>();

	mat3->m_baseColourMap = GemtTex1;
	mat3->m_specularMap = GemSpecTex1;

	mat4->m_baseColourMap = GemtTex2;
	mat4->m_specularMap = GemSpecTex2;

	/*m_model.lock()->SetMaterialBySlot(0, mat3);
	m_model.lock()->SetMaterialBySlot(1, mat4);*/

	/*m_model = ImportModel("Models/Chest/RenderAnim.fbx");
	m_model.lock()->GetTransform().position.x = -15.0f;
	m_model.lock()->GetTransform().rotation.y = 90.0f;
	m_model.lock()->GetTransform().scale = glm::vec3(0.75f);*/

	TShared<STexture> ChestTex = TMakeShared<STexture>();
	ChestTex->LoadTexture("Chest base colour", "Models/Chest/Textures/Chest_BaseColour.tga.png");

	TShared<STexture> ChestSpecTex = TMakeShared<STexture>();
	ChestSpecTex->LoadTexture("Chest spec colour", "Models/Chest/Textures/Chest_Specular.tga.png");

	TShared<STexture> ChestLockTex = TMakeShared<STexture>();
	ChestLockTex->LoadTexture("Chest lock base colour", "Models/Chest/Textures/Head_BaseColour.tga.png");

	TShared<STexture> ChestLockSpecTex = TMakeShared<STexture>();
	ChestLockSpecTex->LoadTexture("Chest lock spec colour", "Models/Chest/Textures/Head_Roughness.tga.png");

	TShared<SSTMaterial> mat5 = TMakeShared<SSTMaterial>();
	TShared<SSTMaterial> mat6 = TMakeShared<SSTMaterial>();

	mat5->m_baseColourMap = ChestTex;
	mat5->m_specularMap = ChestSpecTex;

	mat6->m_baseColourMap = ChestLockTex;
	mat6->m_specularMap = ChestLockSpecTex;

	/*m_model.lock()->SetMaterialBySlot(0, mat5);
	m_model.lock()->SetMaterialBySlot(1, mat6);*/


	/*m_model = ImportModel("Models/Helmet3/Helmet3.fbx");
	m_model.lock()->GetTransform().scale = glm::vec3(0.75f);*/


	// create the dir light
	const auto& dirLight = CreateDirLight();

	// check it exists as a reference
	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(0.0f, 0.0f, 0.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.1f);
		lightRef->intensity = 3.0f;
	}

	const auto& pointLight = CreatePointLight();
	const auto& pointLight2 = CreatePointLight();
	const auto& pointLight3 = CreatePointLight();
	const auto& pointLight4 = CreatePointLight();

	if (const auto& lightRef = pointLight.lock()) {
		lightRef->position = glm::vec3(0.0f, 0.0f, -15.0f);
		lightRef->colour = glm::vec3(1.0f, 0.0f, 0.5f);
		lightRef->intensity = 2.0f;
	}

	if (const auto& lightRef = pointLight2.lock()) {
		lightRef->position = glm::vec3(0.0f, 0.0f, 15.0f);
		lightRef->colour = glm::vec3(0.0f, 1.0f, 0.5f);
		lightRef->intensity = 2.0f;
	}

	if (const auto& lightRef = pointLight3.lock()) {
		lightRef->position = glm::vec3(-20.0f, 0.0f, 0.0f);
		lightRef->colour = glm::vec3(0.0f, 0.5f, 1.0f);
	}

	if (const auto& lightRef = pointLight4.lock()) {
		lightRef->position = glm::vec3(20.0f, 0.0f, 0.0f);
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

	// m_model->GetTransform().position.x = -2.0f;

	//m_model.lock()->GetTransform().rotation.x += 0.01f;
	//m_model.lock()->GetTransform().rotation.y += 0.01f;
	//m_model.lock()->GetTransform().rotation.z += 0.01f;
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
