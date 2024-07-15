#include "Listeners/SInput.h"
#include "SWindow.h"


SInput::SInput()
{
	OnKeyPressed = TMakeShared<SEvents<SDL_Scancode>>();
	OnKeyReleased = TMakeShared<SEvents<SDL_Scancode>>();
	OnMousePressed = TMakeShared<SEvents<>>();
	OnMouseReleased = TMakeShared<SEvents<>>();
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

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			if (const auto& windowRef = m_window.lock())
				m_window.lock()->CloseWindow();
		}

		// run different events depending on the type of input
		switch (e.type) {
		case SDL_KEYDOWN:
			// has this key repeated since it washed down
			if(e.key.repeat == 0)
				OnKeyPressed->Run(e.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			if(e.key.repeat == 0)
				OnKeyReleased->Run(e.key.keysym.scancode);
			break;
		}

	}
}
