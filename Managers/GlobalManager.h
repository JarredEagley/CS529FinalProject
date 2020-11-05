/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GlobalManager.h
	Purpose:			Header file for the GlobalManager.
						This manages the other manager signletons, allowing
						for a single generic point of access.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/20/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "FrameRateController.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Serializer.h"
#include "GraphicsManager.h"

class GlobalManager
{
public:
	static GlobalManager* getInstance()
	{
		if (!instance)
		{
			instance = new GlobalManager;
			instance->initSingletons();
		}
		return instance;
	};
	static void destroySingleton(); // This will destroy all child singeltons. // TO-DO: This needs to be static I think.

public:
	static void initSingletons();
	static FrameRateController* getFrameRateController()	{ return pFRC->getInstance(); };
	static GameObjectManager* getGameObjectManager()		{ return pGOM->getInstance(); };
	static InputManager* getInputManager()					{ return pIM->getInstance(); };
	static ResourceManager* getResourceManager()			{ return pRM->getInstance(); };
	static Serializer* getSerializer()						{ return pSer->getInstance(); };
	static GraphicsManager* getGraphicsManager()			{ return pGraphM->getInstance(); };
private:
	GlobalManager();
private:
	// The global manager's singleton instance.
	static GlobalManager *instance;
	// Using shorthands for these just for convenience.
	static FrameRateController* pFRC;
	static GameObjectManager* pGOM;
	static InputManager* pIM;
	static ResourceManager* pRM;
	static Serializer* pSer;
	static GraphicsManager* pGraphM;
};

