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
#include <vector>
#include <list>

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

	void Update();

	// Note: Target only used for GO_TO, so has a default value of ""
	void handleMenuItemCommand(std::string command, std::string target);

	void destroyMenus();
	
	void displayPauseMenu();
	
	void displayDebugMenu();

	bool testForVictory();
	void displayVictoryMenu();
	void displayDefeatMenu();
	
	float getRandomFloat();

	void readGameConfig();

public:
	enum SceneType
	{
		SCENE_LEVEL,
		SCENE_MENU
	};
	SceneType stringToSceneType(std::string sceneTypeName)
	{
		if (sceneTypeName == "SCENE_LEVEL") return SCENE_LEVEL;
		if (sceneTypeName == "SCENE_MENU") return SCENE_MENU;
		return SCENE_LEVEL;
	}
	SceneType currentSceneType = SCENE_LEVEL;

	std::string currentLevelPath;
	std::string currentLevelName;

	bool mIsGamePaused = false;
	bool mIsLevelLive = false;
	
	// Menu
	
	static std::vector<std::string> mMenuItemNames;


	// Victory condition
	static std::list<std::string> mLivingEnemies;
	bool isPlayerKilled = false;

	// The maximum coordinates before a transform component self destructs its game object.
	float gameCleanupDistance = 500000.0f;


	// Debug flags settable ingame.
	bool DEBUG_DrawCollision = false;

	// Debug flags set in config.
	bool DEBUG_VerboseGameObjects;
	bool DEBUG_VerboseComponents; // Enable or disable non-vital warnings.
	bool DEBUG_VerboseGOF; // Enable or disable non-vital logging from the game object factory.

	std::string initialScene = "MainMenu.json";


private:
	GameStateManager();

private:
	static GameStateManager* instance; // The signleton instance.


};

