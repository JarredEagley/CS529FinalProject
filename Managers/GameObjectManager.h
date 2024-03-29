/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GlobalManager.h
	Purpose:			The header file for the GameObject Manager.
						The vector used to store game objects may
						eventually be replaced with an unordered map
						if deemed appropriate.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once

//#include <vector>
#include <map>
#include "../GameObject.h"

// Will be used to keep track of how many of a type of GameObject may be created
// by the GameObjectFactory at runtime.
enum class GameObjectType
{
	INITIALIZED_AT_START,
	PARTICLE,
	DEBRIS,
	PROJECTILE,

	NUM
};

class GameObjectManager
{
public:
	static GameObjectManager* getInstance()
	{
		if (!instance)
		{
			instance = new GameObjectManager();
		}
		return instance;
	};
	void destroySingleton();

	// Encapsulates checking the map for a gameobject and returning it only if it exists.
	// Prevents nullptrs from being added to mGameObjects.
	GameObject* getGameObject(std::string gameObjectName);

	// Not worth putting in cpp
	int getNextProjectile()
	{
		return (++mProjectileCount) % mMaxProjectiles;
	};

	void updateGameObjects();
	void addCreatedGameObjects(); // Updates mGameObjects
	void deleteRemovedGameObjects(); // Updates mGameObjects

	void deleteAllGameObjects();
	void forceDeleteAllGameObjects();

public:
	static std::map<std::string, GameObject*> mGameObjects; // Should NEVER be modified mid-game loop.

	// These modify mGameObjects at the end of the gameloop.
	static std::list<GameObject*> mNewGameObjects;
	//static std::list<std::string> mMarkedForDelete;

	int mMaxParticles = 100; // Need to data drive these soon.
	int mParticleCount = 0;
	int mMaxProjectiles = 100;
	int mProjectileCount = 0;
	int mMaxDebris = 100;
	int mDebrisCount = 0;


private:
	GameObjectManager(); // Constructor.

private:
	static GameObjectManager* instance;
};

