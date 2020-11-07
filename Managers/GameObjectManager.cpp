/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GlobalManager.cpp
	Purpose:			Implementation details for the GameObject Manager.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "GameObjectManager.h"
//#include "GameObject.h"

#include <iostream>

GameObjectManager* GameObjectManager::instance = nullptr;
std::unordered_map<std::string, GameObject*> GameObjectManager::mGameObjects;

void GameObjectManager::destroySingleton()
{
	// Delete the game object instances.
	for (auto keyValuePair : mGameObjects)
	{
		//std::cout << "DEBUG - INSIDE DESTRUCTOR - " << keyValuePair.first << std::endl;
		delete keyValuePair.second;
	}

	// Clear the map.
	mGameObjects.clear();
		
	// Delete the singleton.
	delete instance;
}

GameObjectManager::GameObjectManager()
{
}
