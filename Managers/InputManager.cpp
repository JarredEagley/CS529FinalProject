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
#include "GlobalManager.h"
#include "memory.h"

#include <iostream>

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

	// Initialize some stuff for the mouse.
	previousMouseState = mousePosYPrev = mousePosXPrev = 0;
	currentMouseState = SDL_GetMouseState(&mousePosXCurr, &mousePosYCurr);
	mouseWheelX = mouseWheelY = 0;
}

void InputManager::Update()
{
	// --- Update Keyboard --- //
	// Save the previous state.
	memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));

	int numberOfFetched_key = 0;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(&numberOfFetched_key);

		if (numberOfFetched_key > 512)
		numberOfFetched_key = 512;

	memcpy(mCurrentState, currentKeyStates, numberOfFetched_key * sizeof(Uint8));

	// --- Update Mouse --- //
	mousePosXPrev = mousePosXCurr;
	mousePosYPrev = mousePosYCurr;
	previousMouseState = currentMouseState;

	currentMouseState = SDL_GetMouseState(&mousePosXCurr, &mousePosYCurr);

	mouseWheelX = 0;
	mouseWheelY = 0;
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
// Useful tutorial:
// http://www.falukdevelop.com/2016/09/07/sdl2_mouse_implementation/

void InputManager::getMousePosition(int& posX, int& posY) const
{
	posX = mousePosXCurr;
	posY = mousePosYCurr;
}

void InputManager::getMouseDiff(int& diffX, int& diffY) const
{
	diffX = mousePosXCurr - mousePosXPrev;
	diffY = mousePosYCurr - mousePosYPrev;
}

// Mouse buttons
bool InputManager::isMouseButtonTriggered(const Uint32 buttonScanCode) const
{
	return ((SDL_BUTTON(buttonScanCode) & currentMouseState) != 0) 
		&& ((SDL_BUTTON(buttonScanCode) & previousMouseState) == 0);
}

bool InputManager::isMouseButtonPressed(const Uint32 buttonScanCode) const
{
	return (SDL_BUTTON(buttonScanCode) & currentMouseState) != 0;
}

bool InputManager::isMouseButtonReleased(const Uint32 buttonScanCode) const
{
	return ((SDL_BUTTON(buttonScanCode) & currentMouseState) == 0)
		&& ((SDL_BUTTON(buttonScanCode) & previousMouseState) != 0);
}

// Mouse wheel
void InputManager::recieveEvent(const SDL_Event& mouseEvent)
{
	// I removed the check for type MouseWheel because that's done in the game loop.
	this->mouseWheelX = mouseEvent.wheel.x;
	this->mouseWheelY = mouseEvent.wheel.y;

	// Broadcast this through my own game event manager.
	MouseScrollEvent* pNewEvent = new MouseScrollEvent();
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewEvent);
}

int InputManager::getWheelX() const
{
	return mouseWheelX;
}

int InputManager::getWheelY() const
{
	return mouseWheelY;
}


