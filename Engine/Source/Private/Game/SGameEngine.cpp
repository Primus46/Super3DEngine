#include "Game/SGameEngine.h"
#include "Game/GameObjects/SObject.h"

// Debug
#include "Game/GameObjects/SObjectChild.h"

SGameEngine* SGameEngine::GetGameEngine()
{
	static SGameEngine* instance = new SGameEngine();

	return instance;
}

void SGameEngine::DestroyGameEngine()
{
	delete GetGameEngine();
}

bool SGameEngine::Run()
{
	if (!Init()) {
		SDebug::Log("Game Engine Failed to Initialise", ST_ERROR);
		return false;
	}

	Start();

	GameLoop();

	return true;
}

void SGameEngine::DestroyObject(const TShared<SObject>& object)
{
	m_objectsPendingDestroyed.push_back(object);
}

SGameEngine::SGameEngine()
{
	m_lastTickTime = 0.0;
	m_deltaTime = 0.0;

	SDebug::Log("Game Engine Created");
}

SGameEngine::~SGameEngine()
{
	CleanUp();

	SDebug::Log("Game Engine Destroyed");
}

bool SGameEngine::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		SDebug::Log("Failed to Initalise SDL: " + SString(SDL_GetError()), ST_ERROR);
		return false;
	}

	// tell SDL that we'll be rendering in gl version 460 or 4.60
	// 4 is the major version 
	// .60 is the minor version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// make sure that if open gl is using a version not in SDL we run SDL in compatibility mode
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	// set the bit depth for each colour
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	// creating window object
	m_window = TMakeShared<SWindow>();

	// creating an sdl window
	if (!m_window->CreateWindow({ "Game window",
		SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		720, 720 }))
		return false;

	// create the input class and assign the window
	m_input = TMakeShared<SInput>();
	m_input->InitInput(m_window);

	return true;
}

void SGameEngine::Start()
{
	// register the window inputs
	m_window->RegisterInput(m_input);

	CreateObject<SObjectChild>().lock()->SetLifeTime(3.0f);
}

void SGameEngine::GameLoop()
{
	// keep the game open as long as the window is open
	while (!m_window->IsPendingClose()) {
		// create delta time
		// set the currect tick time
		// SDL_GetTicks64() gives us time since the engine started in milliseconds
		double curTickTime = static_cast<double>(SDL_GetTicks64());
		// convert the tick time into delta time in milliseconds
		// how much time has passed since the last frame
		double deltaMilli = curTickTime - m_lastTickTime;
		// convert deltaMilli into seconds
		m_deltaTime = deltaMilli / 1000.0;
		// update the last tick time to current time for the next loop
		m_lastTickTime = curTickTime;

		// the order of these functions is important
		// we want to detect input > react tp input with logic > render based on logic
		
		PreLoop();

		// process all engine input functions
		ProcessInput();

		// process all engine tick functions
		Tick();

		// process all engine render functions
		Render();

		PostLoop();
	}
}

void SGameEngine::CleanUp()
{
	m_input = nullptr;
	m_window = nullptr;

	// exit SDL and deallocate all initilised SDL components
	SDL_Quit();	
}

void SGameEngine::Tick()
{
	// run through all SObjects in the game and run their ticks
	for (const auto& SObjectRef : m_objectStack) {
		SObjectRef->Tick(DeltaTimef());
		SObjectRef->PostTick(DeltaTimef());
	}
}

void SGameEngine::ProcessInput()
{
	if (!m_input) {
		return;
	}

	// handle inputs
	m_input->UpdateInputs();
}

void SGameEngine::Render()
{
	if (!m_window) {
		return;
	}

	// render the window
	m_window->Render();
}

void SGameEngine::PreLoop()
{
	// running through all objects to be spawned and running their start logic
	// and adding them into the game object stack
	for (auto& SObjectRef : m_objectsToBeSpawned) {
		SObjectRef->Start();	
		m_objectStack.push_back(std::move(SObjectRef));
	}

	m_objectsToBeSpawned.clear();
}

void SGameEngine::PostLoop()
{ 
	// loop through all objects pending destroy and remove their references from the object stack
	for (const auto& SObjectRef : m_objectsPendingDestroyed) {
		auto it = std::find(m_objectStack.begin(), m_objectStack.end(), SObjectRef);

		if (it == m_objectStack.end()) {
			continue;
		}

		m_objectStack.erase(it);
	}

	// make sure to clear the pending deestroy array so no references of the object exist
	m_objectsPendingDestroyed.clear();
}
