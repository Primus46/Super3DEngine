#include "SWindow.h"
#include "Graphics/SGraphicsEngine.h"
#include "Debugging/SDebug.h"
#include "Listeners/SInput.h"
#include "Graphics/SSTCamera.h"

// External Libs
#include <SDL/SDL.h>


SWindow::SWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;
	m_cameraDirection = glm::vec3(0.0f);
	m_cameraRotation = glm::vec3(0.0f);
	m_canZoom = false;
	m_inputMode = false;

	std::cout << "Window created." << std::endl;
}

SWindow::~SWindow()
{
	// if sdl window exists, destroy it
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	std::cout << "Window destroyed." << std::endl;
}

bool SWindow::CreateWindow(const SSTWindowParams& params)
{
	// enabling opengl in the sdl window
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	// assigning paramaters to the member for the window
	m_params = params;

	// add vsync flag if it's selected
	if (m_params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	// set fullscreen if it's set
	if (m_params.fullscreen)
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP; // Fullscreen borderless
	else
		windowFlags += SDL_WINDOW_SHOWN; // windowed mode

	m_sdlWindow = SDL_CreateWindow(
		m_params.title.c_str(),
		m_params.x,
		m_params.y,
		m_params.w,
		m_params.h,
		windowFlags
	);

	// check if sdl errors are created 
	if (!m_sdlWindow) {
		std::cout << "SDL Failed to create window: " << SDL_GetError() << std::endl;
		CloseWindow();
		return false;
	}

	// creates the graphics engine object
	m_graphicsEngine = std::make_unique<SGraphicsEngine>();

	// initialises the graphics engine and checks
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		SDebug::Log("Window failed to initialised graphics engine", ST_ERROR);
		m_graphicsEngine = nullptr;
		return false;
	}

	return true;
}

void SWindow::RegisterInput(const TShared<SInput>& m_input)
{
	// hide cursor amd set relative mouse mode
	m_input->ShowCursor(false);

	m_input->OnKeyPressed->Bind([this, m_input](const SDL_Scancode& key) {
		// quick exit button for debug
		if (key == SDL_SCANCODE_ESCAPE) {
			CloseWindow();
		}

		// toggle the cursor visibility
		if (key == SDL_SCANCODE_PERIOD) {
			m_input->ShowCursor(m_input->IsCursorHidden());

			// set game to input mode if cursor is visible
			m_inputMode = !m_input->IsCursorHidden();
		}

		if (key == SDL_SCANCODE_W) {
			m_cameraDirection.z += -1.0f;
		}

		if (key == SDL_SCANCODE_S) {
			m_cameraDirection.z += 1.0f;
		}

		if (key == SDL_SCANCODE_A) {
			m_cameraDirection.x += 1.0f;
		}

		if (key == SDL_SCANCODE_D) {
			m_cameraDirection.x += -1.0f;
		}

		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += -1.0f;
		}

		if (key == SDL_SCANCODE_Q) {
			m_cameraDirection.y += 1.0f;
		}
	});

	m_input->OnKeyReleased->Bind([this](const SDL_Scancode& key) {
		if (key == SDL_SCANCODE_W) {
			m_cameraDirection.z += 1.0f;
		}

		if (key == SDL_SCANCODE_S) {
			m_cameraDirection.z += -1.0f;
		}

		if (key == SDL_SCANCODE_A) {
			m_cameraDirection.x += -1.0f;
		}

		if (key == SDL_SCANCODE_D) {
			m_cameraDirection.x += 1.0f;
		}

		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += 1.0f;
		}

		if (key == SDL_SCANCODE_Q) {
			m_cameraDirection.y += -1.0f;
		}
	});

	// On mouse move rotate the camera input
	m_input->OnMouseMove->Bind([this](const float& x, const float& y,
		const float& xrel, const float& yrel) {
		m_cameraRotation.y = -xrel;
		m_cameraRotation.x = -yrel;

		SString log = "X: " + std::to_string(m_cameraRotation.x) +", Y: " + std::to_string(m_cameraRotation.y);
		SDebug::Log(log);
	});

	m_input->OnMouseScroll->Bind([this](const float& delta) {
		if(m_canZoom){
			if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
				camRef->Zoom(delta);
			}
		}
		
	});

	m_input->OnMousePressed->Bind([this](const SUi8& button) {
		if (!m_inputMode) {
			if (button == SDL_BUTTON_RIGHT) {
				m_canZoom = true;
			}
		}
		
	});

	m_input->OnMouseReleased->Bind([this](const SUi8& button) {
		if (!m_inputMode) {
			if (button == SDL_BUTTON_RIGHT) {
				m_canZoom = false;
				if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
					camRef->ResetZoom();
				}
			}
		}
	});

}

void SWindow::Render()
{	
	// render the graphics engine if one exists
	if (m_graphicsEngine) {
		// tes if here is a camera 
		if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
			// translate the camera based on input direction
			camRef->Translate(m_cameraDirection);

			if (!m_inputMode) {
				// rotate the camera based on input direction
				camRef->Rotate(m_cameraRotation, glm::abs(m_cameraRotation));
			}
			
		}

		m_graphicsEngine->Render(m_sdlWindow);
	}
}
