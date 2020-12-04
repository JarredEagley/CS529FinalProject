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

#include <iostream>

GameObjectManager* GameObjectManager::instance = nullptr;
std::map<std::string, GameObject*> GameObjectManager::mGameObjects;
std::list<GameObject*> GameObjectManager::mNewGameObjects;
std::list<std::string> GameObjectManager::mMarkedForDelete;


void GameObjectManager::destroySingleton()
{
	// Delete the game object instances.
	for (auto keyValuePair : mGameObjects)
		delete keyValuePair.second;
	
	// Clear the map.
	mGameObjects.clear();
		
	// Delete the singleton.
	delete instance;
}

GameObjectManager::GameObjectManager()
{
}


GameObject* GameObjectManager::getGameObject(std::string gameObjectName)
{
	// Safely check if entry exists...
	if (mGameObjects.count(gameObjectName) <= 0)
		return nullptr;
	return mGameObjects[gameObjectName];
}


void GameObjectManager::updateGameObjects()
{
	for (auto pGOPair : mGameObjects)
		pGOPair.second->Update();
}


void GameObjectManager::addCreatedGameObjects()
{
	for (auto pGO : mNewGameObjects)
	{
		mGameObjects[pGO->mName] = pGO;
	}
	mNewGameObjects.clear();
}

void GameObjectManager::deleteRemovedGameObjects()
{
	for (auto key : mMarkedForDelete)
	{
		delete mGameObjects[key];
		mGameObjects.erase(key);
	}
}