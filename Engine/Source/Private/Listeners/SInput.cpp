#include "Listeners/SInput.h"
#include "SWindow.h"


SInput::SInput()
{
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
	}
}
