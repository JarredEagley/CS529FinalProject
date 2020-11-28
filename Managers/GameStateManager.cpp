/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameStateManager.cpp
	Purpose:			Implementation of the game state manager
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/27/2020

- End Header --------------------------------------------------------*/

#include "GameStateManager.h"

GameStateManager* GameStateManager::instance = nullptr;

void GameStateManager::destroySingleton()
{
	delete instance;
}

// End of singleton stuff. //

GameStateManager::GameStateManager()
{

}

