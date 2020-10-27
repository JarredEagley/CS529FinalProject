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
#include "SDL_keyboard.h"

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

	int numberOfFetched = 0;
	const Uint8* CurrentKeyStates = SDL_GetKeyboardState(&numberOfFetched);

	if (numberOfFetched > 512)
		numberOfFetched = 512;

	memcpy(mCurrentState, CurrentKeyStates, numberOfFetched * sizeof(Uint8));
}

bool InputManager::IsKeyPressed(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyTriggered(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode] && !mPreviousState[KeyScanCode])
		return true;
	return false;
}

bool InputManager::IsKeyReleased(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;

	if (!mCurrentState[KeyScanCode] && mPreviousState[KeyScanCode])
		return true;
	return false;
}