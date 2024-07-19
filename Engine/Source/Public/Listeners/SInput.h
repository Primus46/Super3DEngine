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

	// On Mouse Move
	// @param1: x position of the mouse
	// @param2: y position of the mouse
	// @param3: x position of the mouse relative to last x position
	// @param4: y position of the mouse relative to last y position
	TShared<SEvents<float, float, float, float>> OnMouseMove;

	// when the mouse scrolls pass the delta
	TShared<SEvents<float>> OnMouseScroll;

	// listen for mouse pressed events
	TShared<SEvents<SUi8>> OnMousePressed;

	// listen for mouse released events
	TShared<SEvents<SUi8>> OnMouseReleased;

	// hide mouse
	// this will also make sure the mouse is centered to the screen when moving
	void ShowCursor(const bool& enable);

	// Get the state of the cursor
	bool IsCursorHidden();


private:
	// weak pionter to the window to not count as a reference
	// so that the input doesn't control if the window gets destroyed or not
	TWeak<SWindow> m_window;

	// last mouse motion event
	SDL_MouseMotionEvent m_lastMotion;

};
