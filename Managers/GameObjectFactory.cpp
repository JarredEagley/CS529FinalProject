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
#include <string>

#include "GlobalManager.h"
#include "../GameObject.h"
#include "../Components/ComponentTypes.h"
#include "../Components/Component.h"

GameObjectFactory* GameObjectFactory::instance = nullptr;

void GameObjectFactory::destroySingleton() 
{
	delete instance;
};

// End of singleton stuff.

GameObjectFactory::GameObjectFactory() { }

void GameObjectFactory::helper_objectRenderPass(GameObject* pGO, const char* renderPassType)
{
	// If this GO Has a render pass aside from final, set its new render pass.
	if (strcmp(renderPassType, "HUD") == 0)
		pGO->setRenderPass(RenderPassType::HUD);
	else
		pGO->setRenderPass(RenderPassType::FINAL);
}

GameObject* GameObjectFactory::loadArchetype(std::string pFileName) 
{
	GameObject* pNewGO;
	std::string componentName;

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


	// Set shader via archetype if specified.
	if (
		doc.HasMember("Shader")
		&& doc["Shader"].IsString()
		)
	{
		std::string newShaderName = doc["Shader"].GetString();
		pNewGO->mShaderName = newShaderName;
	}

	// Set render pass via archetype if its specified.
	if (
		doc.GetObject().HasMember("Render Pass")
		&& doc["Render Pass"].IsString()
		)
		helper_objectRenderPass(pNewGO, doc["Render Pass"].GetString());
	// Remain NONE otherwise.

	return pNewGO;
}

GameObject* GameObjectFactory::loadObject(rapidjson::GenericObject<true, rapidjson::Value> inputObj)
{
	// Get the name of the current GO. 
	std::string currentGOName;
	if (!inputObj.HasMember("Name") || !inputObj["Name"].IsString())
	{
		std::cout << "Error: GameObject in level failed to deserialize because it lacked a name or contained an invalid name." << std::endl;
		return nullptr;
	}

	currentGOName = inputObj["Name"].GetString();
	std::cout << "Deserializing GameObject " << currentGOName << std::endl;

	// Check if GO by this name aready exists...
	if (GlobalManager::getGameObjectManager()->mGameObjects.find(currentGOName) != GlobalManager::getGameObjectManager()->mGameObjects.end())
		std::cout << "Warning: GameObject by name " << currentGOName << " already exists and will be replaced." << std::endl;


	GameObject* pCurrentGO = nullptr;

	// Load the archetype if one is provided and ensure its a string.
	if (inputObj.HasMember("Archetype")
		&& inputObj["Archetype"].IsString())
	{
		// Make sure archetype is valid.
		const char* archetypeName = inputObj["Archetype"].GetString();
		std::string pathName{ archetypeName };
		pathName = GlobalManager::getResourceManager()->pathArchetypes + pathName;

		pCurrentGO = this->loadArchetype(pathName.c_str());
	}

	// Two possibilities: Archetype was invalid or archetype didn't deserialize correctly.
	if (nullptr == pCurrentGO)
	{
		// Otherwise just create a fresh GameObject.
		std::cerr << "Warning: Archetype for GameObject was invalid or not present." << std::endl;
		pCurrentGO = new GameObject;
	}

	// Name
	pCurrentGO->mName = currentGOName;

	// Test if theres additioanl components, ensure its valid format.
	if (
		inputObj.HasMember("Components")
		&& inputObj["Components"].IsObject()
		)
	{
		// Loop through the components and add them to the object. (Or overwrite them)
		for (rapidjson::Value::ConstMemberIterator compItr = inputObj["Components"].GetObject().MemberBegin();
			compItr != inputObj["Components"].GetObject().MemberEnd();
			++compItr)
		{
			// Check this components' validity.
			if (!compItr->name.IsString())
			{
				std::cerr << "Warning: Failed to parse a component while building level's GameObjects." << std::endl;
				continue;
			}

			std::cout << "Note: (re)loading component " << compItr->name.GetString() << std::endl;

			// Deserialize this component.
			Component* pCurrentComp = pCurrentGO->AddComponent(ComponentTypes::stringToEnum(compItr->name.GetString()));
			pCurrentComp->Serialize(compItr);
		}
	}

	// If this GO is to be parented, parent it.
	if (
		inputObj.HasMember("Parent")
		&& inputObj["Parent"].IsString()
		)
	{
		std::string parentName = inputObj["Parent"].GetString();
		GameObject* pParent = GlobalManager::getGameObjectManager()->mGameObjects[parentName];

		// Make sure parent wasn't invalid.
		if (pParent == nullptr)
			std::cout << "Warning: Parent was either invalid or not initialized before child GameObject. Parenting failed for " << currentGOName << std::endl;
		else
			pCurrentGO->setParent(pParent);
	}

	// If this GO is to have another shader aside from core, set its new shader.
	if (
		inputObj.HasMember("Shader")
		&& inputObj["Shader"].IsString()
		)
	{
		std::string newShaderName = inputObj["Shader"].GetString();
		pCurrentGO->mShaderName = newShaderName;
	}

	// Is render pass already set by archetype?
	if (pCurrentGO->getRenderPassType() == RenderPassType::NONE)
	{
		// If this GO Has a render pass aside from final, set its new render pass.
		if (
			inputObj.HasMember("Render Pass")
			&& inputObj["Render Pass"].IsString()
			)
			helper_objectRenderPass(pCurrentGO, inputObj["Render Pass"].GetString());
		else
			pCurrentGO->setRenderPass(RenderPassType::FINAL);
	}

	// Initialize.
	pCurrentGO->initializeComponents();

	// Push onto the GOM and return.
	GlobalManager::getGameObjectManager()->mGameObjects[currentGOName] = pCurrentGO;
	return pCurrentGO;
}


GameObject* GameObjectFactory::generateProjectile(std::string pFileName)
{
	GameObject* pNewGO;
	std::string componentName;

	pFileName = GlobalManager::getResourceManager()->pathProjectiles + pFileName;

	Serializer* pSer = GlobalManager::getSerializer();
	rapidjson::Document doc = pSer->loadJson(pFileName);

	// Nullcheck the document.
	if (doc.IsNull() || !doc.IsObject())
	{
		// Game object creation failed.
		std::cerr << "Error: Failed to load dynamic GameObject file " << pFileName << std::endl;
		return nullptr; // Return nullptr on fail.
	}

	// Make sure we're deserializing a GameObject. 
	if (!doc.HasMember("Type")
		|| !doc["Type"].IsString()
		|| strcmp(doc["Type"].GetString(), "GameObject") != 0)
	{
		// We're not deserializing a GameObject.
		std::cerr << "Error: File " << pFileName << " was not of type GameObject." << std::endl;
		return nullptr; // Return nullptr on fail.
	}

	// Get the name of this GO. 
	std::string newGOName;
	if (!doc.HasMember("Name") || !doc["Name"].IsString())
	{
		std::cout << "Error: Dynamic GameObject failed to deserialize because it lacked a name or contained an invalid name." << std::endl;
		return nullptr;
	}


	newGOName = doc["Name"].GetString();
	newGOName = "PROJECTILE_" + newGOName;// GlobalManager::getGameObjectManager()->mProjectileCount;
	newGOName = newGOName + "_" + std::to_string(GlobalManager::getGameObjectManager()->mProjectileCount);
	std::cout << "Deserializing GameObject " << newGOName << std::endl; // Having this uncommented might get obnoxious.

	// I would like to delegate this to the game object manager, maybe. It's a bit ugly this way.
	GlobalManager::getGameObjectManager()->mProjectileCount += 1;
	GlobalManager::getGameObjectManager()->mProjectileCount = GlobalManager::getGameObjectManager()->mProjectileCount 
		% GlobalManager::getGameObjectManager()->mMaxParticles;

	// Create the GameObject and proceed to adding components.
	pNewGO = new GameObject();

	// Name
	pNewGO->mName = newGOName;

	// Make sure components exists.
	if (!doc.HasMember("Components") || !doc["Components"].IsObject())
	{
		// No components to add to the game object. This is an empty game object.
		std::cerr << "Warning: Dynamic GameObject " << pFileName << " did not contain any components." << std::endl;
		return pNewGO; // Return empty GameObject since it was a valid GameObject but had no components.
	}
	
	// Parse through components.
	rapidjson::Value::ConstMemberIterator itr = doc["Components"].GetObject().MemberBegin();
	for (; itr != doc["Components"].GetObject().MemberEnd(); ++itr)
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


	// Set shader via archetype if specified.
	/*
	if (
		doc.HasMember("Shader")
		&& doc["Shader"].IsString()
		)
	{
		std::string newShaderName = doc["Shader"].GetString();
		pNewGO->mShaderName = newShaderName;
	}
	*/

	// Initialize components.
	pNewGO->initializeComponents();

	// Push onto the GameObjectManager and return.
	//GlobalManager::getGameObjectManager()->mGameObjects[newGOName] = pNewGO; // Bad bad bad.
	GlobalManager::getGameObjectManager()->mNewGameObjects.push_back(pNewGO);

	// I'm going to just assume all Dynamic GO's use final render pass.
	pNewGO->setRenderPass(RenderPassType::FINAL);

	// Automatically subscribe this to projectile events.
	GlobalManager::getEventManager()->Subscribe(EventType::DESTROY_PROJETILE, pNewGO);

	return pNewGO;
}

