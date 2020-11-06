/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			inputmanager.cpp
	Purpose:			Contains the class methods for the input manager. 
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "inputmanager.h"
#include "memory.h"

InputManager* InputManager::instance = nullptr;

void InputManager::destroySingleton()
{
	delete instance;
}


InputManager::InputManager()
{
	// Destination, value, # of bytes. NO ALLOCATION OCCURS.
	memset(mCurrentState, 0, 512 * sizeof(Uint8));
	memset(mPreviousState, 0, 512 * sizeof(Uint8));
}

void InputManager::Update()
{
	// Save the previous state.
	memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));

	int numberOfFetched_key = 0;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(&numberOfFetched_key);

	//const Uint32 currentMouseStates = SDL_GetMouseState(&mouseX, &mouseY);

	if (numberOfFetched_key > 512)
		numberOfFetched_key = 512;

	memcpy(mCurrentState, currentKeyStates, numberOfFetched_key * sizeof(Uint8));
}

bool InputManager::IsKeyPressed(unsigned int keyScanCode)
{
	if (keyScanCode >= 512)
		return false;

	if (mCurrentState[keyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyTriggered(unsigned int keyScanCode)
{
	if (keyScanCode >= 512)
		return false;

	if (mCurrentState[keyScanCode] && !mPreviousState[keyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyReleased(unsigned int keyScanCode)
{
	if (keyScanCode >= 512)
		return false;

	if (!mCurrentState[keyScanCode] && mPreviousState[keyScanCode])
		return true;
	return false;
}

// --- MOUSE --- //

void InputManager::getMousePosition(int& posX, int& posY) const
{

}

void InputManager::getMouseDiff(int& diffX, int& diffY) const
{

}
// Mouse buttons
bool InputManager::isMouseButtonTriggered(const Uint32 button) const
{

}

bool InputManager::isMouseButtonPressed(const Uint32 button) const
{

}

bool InputManager::isMouseButtonReleased(const Uint32 button) const
{

}

// Mouse wheel
void InputManager::recieveEvent(const SDL_Event& event)
{

}

int InputManager::getWheelX() const
{

}

int InputManager::getWheelY() const
{

}