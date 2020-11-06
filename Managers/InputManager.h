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
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include "SDL_events.h"

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

	// Keyboard
	bool IsKeyTriggered(unsigned int keyScanCode);
	bool IsKeyPressed(unsigned int keyScanCode);
	bool IsKeyReleased(unsigned int keyScanCode);

	
	// Mouse position
	void getMousePosition(int& posX, int& posY) const;
	void getMouseDiff(int& diffX, int& diffY) const;
	// Mouse buttons
	bool isMouseButtonTriggered(const Uint32 buttonScanCode) const;
	bool isMouseButtonPressed(const Uint32 buttonScanCode) const;
	bool isMouseButtonReleased(const Uint32 buttonScanCode) const;

	// Mouse wheel
	void recieveEvent(const SDL_Event& mouseEvent); // Used to recieve SDL_MouseWheelEvent
	int getWheelX() const;
	int getWheelY() const;


public:
	// No public variables.
private:
	InputManager();
private:
	static InputManager* instance;
	Uint8 mCurrentState[512];
	Uint8 mPreviousState[512];
	// Mouse
	int mousePosXCurr;
	int mousePosYCurr;
	int mousePosXPrev;
	int mousePosYPrev;
	Uint32 currentMouseState;
	Uint32 previousMouseState;
	Sint32 mouseWheelX;
	Sint32 mouseWheelY;

};