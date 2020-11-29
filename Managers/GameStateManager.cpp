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
#include "GlobalManager.h"

GameStateManager* GameStateManager::instance = nullptr;

void GameStateManager::destroySingleton()
{
	delete instance;
}

// End of singleton stuff. //

GameStateManager::GameStateManager()
{
}


void readGameConfig()
{
	// Use serializer to read the json in.
	Serializer* pSer = GlobalManager::getSerializer();
	rapidjson::Document doc = pSer->loadJson(GlobalManager::getResourceManager()->pathConfig.c_str());

	//if (doc.HasMember("Gamestate Manager") && doc["Gamestate Manager"].IsObject())
	{
	}

	if (doc.HasMember("Framerate Controller") && doc["Framerate Controller"].IsObject())
	{
		auto currentObj = doc["Framerate Controller"].GetObject();
		if (currentObj.HasMember("Max Framerate") && currentObj["Max Framerate"].IsNumber())
		{
			GlobalManager::getFrameRateController()->setMaxFramerate(
				currentObj["Max Framerate"].GetInt()
			);
		}
	}

	//if (doc.HasMember("GameObject Manager") && doc["GameObject Manager"].IsObject())
	{
	}

	//if (doc.HasMember("Input Manager") && doc["Input Manager"].IsObject())
	{
	}

	//if (doc.HasMember("Resource Manager") && doc["Resource Manager"].IsObject())
	{
	}

	//if (doc.HasMember("Serializer") && doc["Serializer"].IsObject())
	{
	}

	//if (doc.HasMember("GameObject Factory") && doc["GameObject Factory"].IsObject())
	{
	}

	if (doc.HasMember("Graphics Manager") && doc["Graphics Manager"].IsObject())
	{
		auto currentObj = doc["Graphics Manager"].GetObject();

		// Width and height.
		const char* currentattr = "Window Height";
		if (currentObj.HasMember(currentattr) && currentObj[currentattr].IsNumber())
		{
			GlobalManager::getGraphicsManager()->mWindowHeight = currentObj[currentattr].GetInt();
		}

		const char* currentattr = "Window Width";
		if (currentObj.HasMember(currentattr) && currentObj[currentattr].IsNumber())
		{
			GlobalManager::getGraphicsManager()->mWindowWidth = currentObj[currentattr].GetInt();
		}

		// Zoomer stuff
		const char* currentattr = "Zoom Level";
		if (currentObj.HasMember(currentattr) && currentObj[currentattr].IsNumber())
		{
			GlobalManager::getGraphicsManager()->setZoomLevel( currentObj[currentattr].GetFloat() );
		}

		const char* currentattr = "Max Zoom Level";
		if (currentObj.HasMember(currentattr) && currentObj[currentattr].IsNumber())
		{
			GlobalManager::getGraphicsManager()->setMaxZoomLevel( currentObj[currentattr].GetFloat() );
		}

		const char* currentattr = "Min Zoom Level";
		if (currentObj.HasMember(currentattr) && currentObj[currentattr].IsNumber())
		{
			GlobalManager::getGraphicsManager()->setMinZoomLevel( currentObj[currentattr].GetFloat() );
		}
	}

	//if (doc.HasMember("Collision Manager") && doc["Collision Manager"].IsObject())
	{
	}

	//if (doc.HasMember("Event Manager") && doc["Event Manager"].IsObject())
	{

	}
}


