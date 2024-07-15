#pragma once
#include "EngineTypes.h"
#include "Listeners/SEvents.h"

// External Libs
#include "SDL/SDL_keyboard.h"
#include "SDL/SDL_events.h"

class SWindow;

class SInput {
public:
	SInput();

	// Initialises the window
	void InitInput(const TShared<SWindow>& Window);

	// Updating the inputs
	void UpdateInputs();

	// On a pressed event
	TShared<SEvents<SDL_Scancode>> OnKeyPressed;

	// On a released event
	TShared<SEvents<SDL_Scancode>> OnKeyReleased;

	// On a pressed event
	TShared<SEvents<>> OnMousePressed;

	// On a released event
	TShared<SEvents<>> OnMouseReleased;


private:
	// weak pionter to the window to not count as a reference
	// so that the input doesn't control if the window gets destroyed or not
	TWeak<SWindow> m_window;

};
