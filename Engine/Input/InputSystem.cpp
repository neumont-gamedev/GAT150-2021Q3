#include "InputSystem.h"
#include <iostream>

namespace nc
{
	void InputSystem::Startup()
	{
		// get pointer to SDL keyboard states
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);

		// set size of keyboard state
		keyboardState.resize(numKeys);
		// copy SDL keyboard state to keystate
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());
		// set prev key state to current key state
		prevKeyboardState = keyboardState;
	}

	void InputSystem::Shutdown()
	{
		//
	}

	void InputSystem::Update(float dt)
	{
		// set the previous keyboard state to keyboard state
		prevKeyboardState = keyboardState;
		// copy the current SDL keyboard state to keyboard state
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

		prevMouseButtonState = mouseButtonState;
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = nc::Vector2{ x , y };
		mouseButtonState[0] = buttons & SDL_BUTTON_LMASK; // buttons [0001] & [0RML]
		mouseButtonState[1] = buttons & SDL_BUTTON_MMASK; // buttons [0010] & [0RML]
		mouseButtonState[2] = buttons & SDL_BUTTON_RMASK; // buttons [0100] & [0RML]
	}

	InputSystem::eKeyState InputSystem::GetKeyState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);

		if (keyDown)
		{
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? eKeyState::Released : eKeyState::Idle;
		}

		return state;
	}

	InputSystem::eKeyState InputSystem::GetButtonState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool keyDown = IsButtonDown(id);
		bool prevKeyDown = IsPreviousButtonDown(id);

		if (keyDown)
		{
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? eKeyState::Released : eKeyState::Idle;
		}

		return state;
	}
}