/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			inputmanager.h
	Purpose:			Defines the class for the input manager.
						This is now a singleton.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "SDL_stdinc.h"

class InputManager
{
public:
	static InputManager* getInstance()
	{
		if (!instance)
			instance = new InputManager;
		return instance;
	};
	void destroySingleton();

	void Update();

	bool IsKeyPressed(unsigned int KeyScanCode);
	bool IsKeyTriggered(unsigned int KeyScanCode);
	bool IsKeyReleased(unsigned int KeyScanCode);
public:
	// No public variables.
private:
	InputManager();
private:
	static InputManager* instance;
	Uint8 mCurrentState[512];
	Uint8 mPreviousState[512];
};