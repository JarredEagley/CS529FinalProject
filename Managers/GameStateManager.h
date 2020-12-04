/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameStateManager.h
	Purpose:			The game state manager controls the state and
						flow of the game. This includes level loading,
						level initialization, pausing, ect.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/27/2020

- End Header --------------------------------------------------------*/

#pragma once

#include <random>
#include <time.h>

class GameStateManager
{
public:
	static GameStateManager* getInstance()
	{
		if (!instance)
			instance = new GameStateManager;
		return instance;
	};
	void destroySingleton();

	float getRandomFloat();

	void readGameConfig();

public:
	enum sceneType
	{
		SCENE_LEVEL,
		SCENE_MENU
	};
	// Debug flags.
	bool DEBUG_VerboseGameObjects = false;
	bool DEBUG_VerboseComponents = false; // Enable or disable non-vital warnings.
	bool DEBUG_VerboseGOF = false; // Enable or disable non-vital logging from the game object factory.

private:
	GameStateManager();

private:
	static GameStateManager* instance; // The signleton instance.
};

