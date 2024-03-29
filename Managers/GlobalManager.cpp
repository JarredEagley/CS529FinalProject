/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GlobalManager.h
	Purpose:			The implementation details for GlobalManager.h.
						This ensures all the managers are initialized
						and destroyed correctly.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/20/2020

- End Header --------------------------------------------------------*/

#include "GlobalManager.h"

#include <iostream>

GlobalManager*			GlobalManager::instance = nullptr;

GameStateManager*		GlobalManager::pGSM = nullptr;
FrameRateController*	GlobalManager::pFRC = nullptr;
GameObjectManager*		GlobalManager::pGOM = nullptr;
InputManager*			GlobalManager::pIM = nullptr;
ResourceManager*		GlobalManager::pRM = nullptr;
Serializer*				GlobalManager::pSer = nullptr;
GraphicsManager*		GlobalManager::pGraphM = nullptr;
GameObjectFactory*		GlobalManager::pGOFactory = nullptr;;
PhysicsManager*			GlobalManager::pPhysM = nullptr;
CollisionManager*		GlobalManager::pColM = nullptr;
EventManager*			GlobalManager::pEM = nullptr;

// Fires all the getters to initialze all singletons GM is responsible for.
void GlobalManager::initSingletons()
{
	getGameStateManager();
	getFrameRateController();
	getGameObjectManager();
	getInputManager();
	getResourceManager();
	getSerializer();
	getGraphicsManager();
	getGameObjectFactory();
	getPhysicsManager();
	getCollisionManager();
	getEventManager();
}

// Call the destroy method on every singleton. 
void GlobalManager::destroySingleton()
{
	// I'm sure there's a better way of doing this, but this is good enough for now.
	pGSM->destroySingleton();
	pFRC->destroySingleton();
	pGOM->destroySingleton();
	pIM->destroySingleton();
	pRM->destroySingleton();
	pSer->destroySingleton();
	pGraphM->destroySingleton();
	pGOFactory->destroySingleton();
	pPhysM->destroySingleton();
	pColM->destroySingleton();
	pEM->destroySingleton();

	delete instance;
}


