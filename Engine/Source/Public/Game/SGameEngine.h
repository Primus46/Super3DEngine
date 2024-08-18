#pragma once
#include "EngineTypes.h"
#include "SWindow.h"
#include "Listeners/SInput.h"

// External Libs
#include <SDL/SDL.h>

class SObject;

class SGameEngine {
public:
	// get or create the game engine if one doesn't exist
	static SGameEngine* GetGameEngine();

	// destroy the game engine
	static void DestroyGameEngine();

	// run the game engine
	bool Run();

	// return the delta time between frames
	double DeltaTime() const { return m_deltaTime; }

	// return the delta time between frames as a float
	float DeltaTimef() const { return static_cast<float>(m_deltaTime); }

	// create a SObject type
	template<typename T, std::enable_if_t<std::is_base_of_v<SObject, T>, T>* = nullptr> 
	TWeak<T> CreateObject() {
		// create an object with the template class
		TShared<T> newObject = TMakeShared<T>();

		// add the object into the stack
		m_objectsToBeSpawned.push_back(newObject);

		return newObject;
	}

	// mark an object for destroy
	// all game objeects destroy functions will automatically run this
	void DestroyObject(const TShared<SObject>& object);

	// get the graphics engine
	TUnique<SGraphicsEngine>& GetGraphics();

private:
	// constructor and destructor are private to ensure we can only have 1 game engine
	SGameEngine();
	~SGameEngine();

	// initialise all required libraries for the game engine
	bool Init();

	// runs after initalise of the game engine
	void Start();

	// run the loop of the gmae
	void GameLoop();

	// clean up the game engine
	void CleanUp();

	// LOOP FUNCTIONS
	// Run game logic for the frame
	void Tick();

	// process the input for each frame
	void ProcessInput();

	// render the graphics for each frame
	void Render();	

	// runs at the start of each loop
	void PreLoop();

	// runs at the end of each loop
	void PostLoop();

	

private:
	// store the window for the game engine
	TShared<SWindow> m_window;

	// store the input for the game engine
	TShared<SInput> m_input;

	// last time ticked
	double m_lastTickTime;

	// delta time between frames
	double m_deltaTime;

	// store all SObjects in the game engine
	TArray<TShared<SObject>> m_objectStack;

	// store all SObjects to be started next frame
	TArray<TShared<SObject>> m_objectsToBeSpawned;

	// store all SObjects that have been marked for destroy
	TArray<TShared<SObject>> m_objectsPendingDestroyed;

	int m_destroyedObjectsCount;
};