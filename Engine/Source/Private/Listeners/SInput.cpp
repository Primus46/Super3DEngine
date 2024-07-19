#include "Listeners/SInput.h"
#include "SWindow.h"


SInput::SInput()
{
	OnKeyPressed = TMakeShared<SEvents<SDL_Scancode>>();
	OnKeyReleased = TMakeShared<SEvents<SDL_Scancode>>();
	OnMouseMove = TMakeShared<SEvents<float, float, float, float>>();
	OnMouseScroll = TMakeShared<SEvents<float>>();
	OnMousePressed = TMakeShared<SEvents<SUi8>>();
	OnMouseReleased = TMakeShared<SEvents<SUi8>>();
	m_lastMotion = SDL_MouseMotionEvent();
}

void SInput::InitInput(const TShared<SWindow>& Window)
{
	// to assign a weak pointer, just plug in the shared pointer
	m_window = Window;
}

void SInput::UpdateInputs()
{
	// TO DO: Game Loop
	SDL_Event e;
	bool mouseMoved = false;	

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			if (const auto& windowRef = m_window.lock())
				m_window.lock()->CloseWindow();
		}

		// run different events depending on the type of input
		switch (e.type) {
		case SDL_KEYDOWN:
			// has this key repeated since it washed down
			if (e.key.repeat == 0)
				OnKeyPressed->Run(e.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			if (e.key.repeat == 0)
				OnKeyReleased->Run(e.key.keysym.scancode);
			break;
		case SDL_MOUSEMOTION:
			// when the mouse moves run the mouse move event
			// this only runs when the mouse moves so we need another point...
			// ...to dewtermine when the mouse has stopped motion
			OnMouseMove->Run(
				static_cast<float>(e.motion.x),
				static_cast<float>(e.motion.y),
				static_cast<float>(e.motion.xrel),
				static_cast<float>(e.motion.yrel)
			);

			// set the last motion for stopping
			m_lastMotion = e.motion;
			mouseMoved = true;
			break;
		case SDL_MOUSEWHEEL:
			// run the scroll event when the mouse wheel scrolls
			OnMouseScroll->Run(e.wheel.preciseY);
			break;
		case SDL_MOUSEBUTTONDOWN:
			OnMousePressed->Run(e.button.button);
		break;
		case SDL_MOUSEBUTTONUP:
			OnMouseReleased->Run(e.button.button);
		break;
		}

	}

	// if the mouse hasn't moved and the last motion was movement
	// run the mouse move function and 0 out releative movements
	if (!mouseMoved && (m_lastMotion.xrel != 0 || m_lastMotion.yrel != 0)) {
		OnMouseMove->Run(
			static_cast<float>(m_lastMotion.x),
			static_cast<float>(m_lastMotion.y),
			0.0f, 0.0f
		);

		m_lastMotion.xrel = 0;
		m_lastMotion.yrel = 0;
	}
}

void SInput::ShowCursor(const bool& enable)
{
	// hide the mouse and set the cursor to the center of the screen if true
	// using shorthand if statement to get result
	// [condition] ? [result if true] : [return if false]
	SDL_SetRelativeMouseMode(enable ? SDL_FALSE : SDL_TRUE);
}

bool SInput::IsCursorHidden()
{
	return SDL_GetRelativeMouseMode() == SDL_TRUE ? true : false;
}
