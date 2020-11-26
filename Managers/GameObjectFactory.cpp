/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameObjectFactory.cpp
	Purpose:			My implementation for the GameObject Factory.
						and/or altered in the future, but right now handles:
							- Loading Objects
							- Loading Levels
						Some edge cases may be missed.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/23/2020

- End Header --------------------------------------------------------*/

#include "GameObjectFactory.h"
#include <iostream>
#include <fstream>

#include "GlobalManager.h"
#include "../GameObject.h"
#include "../Components/ComponentTypes.h"
#include "../Components/Component.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

GameObjectFactory* GameObjectFactory::instance = nullptr;

void GameObjectFactory::destroySingleton() {};

GameObjectFactory::GameObjectFactory() { }

GameObject* GameObjectFactory::loadObject(const char* pFileName) 
{
	GameObject* pNewGO;
	//std::string gameObjectName;
	std::string componentName;
	std::ifstream inputStream(pFileName); 

	Serializer* pSer = GlobalManager::getSerializer();
	rapidjson::Document doc = pSer->loadJson(pFileName);

	// Nullcheck the document.
	if (doc.IsNull())
	{
		// Game object creation failed.
		std::cerr << "Error: Failed to load GameObject file " << pFileName << std::endl;
		return nullptr; // Return nullptr on fail.
	}

	// Make sure we're deserializing a GameObject. 
	if ( !doc.HasMember("Type") 
		|| !doc["Type"].IsString() 
		|| strcmp(doc["Type"].GetString(), "GameObject") != 0)
	{
		// We're not deserializing a GameObject.
		std::cerr << "Error: File " << pFileName << " was not of type GameObject." << std::endl;
		return nullptr; // Return nullptr on fail.
	}

	/*
	// Get this GO's UNIQUE name.
	// MUST BE UNIQUE.
	if (!doc.HasMember("Name")
		|| !doc["Name"].IsString())
	{
		std::cerr << "Error: File " << pFileName << " did not have any valid unique name identifier." << std::endl;
		return nullptr; 
	}

	gameObjectName = doc["Name"].GetString();

	std::cout << "DEBUG - Deserializing name " << gameObjectName << std::endl;
	
	// Check the map for repeats.
	if (GlobalManager::getGameObjectManager()->mGameObjects.find(gameObjectName.c_str()) != GlobalManager::getGameObjectManager()->mGameObjects.end())
	{
		std::cout << "Note: GameObject with name '" << gameObjectName << "' already exists and will be replaced." << std::endl;
	}
	*/

	
	// Create the new GameObject now that we know the json is reasonable.
	pNewGO = new GameObject; 

	// Make sure components exists.
	if (!doc.HasMember("Components") || !doc["Components"].IsObject())
	{
		// No components to add to the game object. This is an empty game object.
		std::cerr << "Warning: GameObject " << pFileName << " did not contain any components." << std::endl;
		return pNewGO; // Return empty GameObject since it was a valid GameObject but had no components.
	}

	// Parse through components.
	rapidjson::Value::ConstMemberIterator itr = doc["Components"].GetObject().MemberBegin();
	for (; itr != doc["Components"].GetObject().MemberEnd(); ++itr )
	{
		if (itr->name.IsString() == false)
		{
			std::cout << "Warning: Failed to deserialize a component because. itr->name was not string." << std::endl;
			continue;
		}

		Component* pNewComponent = nullptr;
		std::cout << "Reading in: " << itr->name.GetString() << "\n";
		pNewComponent = pNewGO->AddComponent(ComponentTypes::stringToEnum(itr->name.GetString()));

		std::cout << "Done.\n";

		// Rapidjson didn't like me passing just the value in, so pass the whole iterator.
		if (nullptr != pNewComponent)
			pNewComponent->Serialize(itr);
	}

	return pNewGO;
}

void GameObjectFactory::loadLevel(const char* pFileName)
{
	std::ifstream inputStream(pFileName);

	Serializer* pSer = GlobalManager::getSerializer();
	rapidjson::Document doc = pSer->loadJson(pFileName);

	// Nullcheck.
	if (doc.IsNull())
	{
		std::cerr << "Error: Failed to load level with filename " << pFileName << std::endl;
		return;
	}

	// Make sure we're deserializing a level.
	if (!doc.HasMember("Type") || !doc["Type"].IsString() || strcmp(doc["Type"].GetString(), "Level") != 0)
	{
		std::cerr << "Error: File " << pFileName << " is not of type 'Level'" << std::endl;
		return;
	}

	// Make sure this level has GameObjects to parse.
	// Note: GameObjects member is an array/list of documents.
	if (!doc.HasMember("GameObjects") || !doc["GameObjects"].IsArray())
	{
		std::cerr << "Warning: File " << pFileName << " did not contain any GameObjects." << std::endl;
		return;
	}

	// Loop through the GameObjects array.
	for (rapidjson::Value::ConstValueIterator arrItr = doc["GameObjects"].GetArray().Begin();
		arrItr != doc["GameObjects"].GetArray().End();
		++arrItr)
	{
		// Make sure this member is an object.
		if (!arrItr->IsObject())
		{
			std::cout << "Warning: Failed to parse an object while loading level." << std::endl;
			continue;
		}

		// Get the name of the current GO. 
		std::string currentGOName;
		if (!arrItr->HasMember("Name") || !arrItr->GetObject()["Name"].IsString())
		{
			std::cout << "Error: GameObject in level failed to deserialize because it lacked a name or contained an invalid name." << std::endl;
			continue;
		}
		currentGOName = arrItr->GetObject()["Name"].GetString();
		// Check if GO by this name already exists.

		std::cout << "Deserializing GameObject " << currentGOName << std::endl;

		GameObject *pCurrentGO = nullptr;

		// Load the archetype if one is provided and ensure its a string.
		if (arrItr->GetObject().HasMember("Archetype") 
			&& arrItr->GetObject().FindMember("Archetype")->value.IsString())
		{
			// Make sure archetype is valid.
			const char* archetypeName = arrItr->GetObject().FindMember("Archetype")->value.GetString();
			std::string pathName{ archetypeName };
			pathName = GlobalManager::getResourceManager()->pathArchetypes + pathName; 
			pCurrentGO = loadObject(pathName.c_str());
		}
		
		// Two possibilities: Archetype was invalid or archetype didn't deserialize correctly.
		if (nullptr == pCurrentGO)
		{
			// Otherwise just create a fresh GameObject.
			std::cerr << "Warning: Archetype for GameObject was invalid or not present." << std::endl;
			pCurrentGO = new GameObject;
		}

		// Test if theres additioanl components, ensure its valid format.
		if (arrItr->GetObject().HasMember("Components")
			&& arrItr->GetObject().FindMember("Components")->value.IsObject())
		{
			// Loop through the components and add them to the object. (Or overwrite them)
			for (rapidjson::Value::ConstMemberIterator compItr = arrItr->GetObject().FindMember("Components")->value.MemberBegin();
				compItr != arrItr->GetObject().FindMember("Components")->value.MemberEnd(); 
				++compItr)
			{
				// Check this components' validity.
				if (!compItr->name.IsString())
				{
					std::cerr << "Warning: Failed to parse a component while building level's GameObjects." << std::endl;
					continue;
				}

				std::cout << "DEBUG - (re)loading component " << compItr->name.GetString() << std::endl;
				
				// Deserialize this component.
				Component* pCurrentComp = pCurrentGO->AddComponent(ComponentTypes::stringToEnum(compItr->name.GetString()));
				pCurrentComp->Serialize(compItr);

			}
		}

		// If this GO is to be parented, parent it.
		if (
			arrItr->GetObject().HasMember("Parent")
			&& arrItr->GetObject()["Parent"].IsString()
			)
		{
			std::string parentName = arrItr->GetObject()["Parent"].GetString();
			GameObject* pParent = GlobalManager::getGameObjectManager()->mGameObjects[parentName];

			// Make sure parent wasn't invalid.
			if (pParent == nullptr)
				std::cout << "Warning: Parent was either invalid or not initialized before child GameObject. Parenting failed for " << currentGOName << std::endl;
			else
				pCurrentGO->setParent(pParent);
		}

		// Name
		pCurrentGO->mName = currentGOName;
		
		// Push onto the GOM.
		GlobalManager::getGameObjectManager()->mGameObjects[currentGOName] = pCurrentGO;
	}

}
