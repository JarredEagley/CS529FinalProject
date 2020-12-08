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


// Pushes onto the game object manager.
void GameObjectFactory::helper_initializeObject(GameObject* pGO)
{
	pGO->initializeComponents();
	GlobalManager::getGameObjectManager()->mNewGameObjects.push_back(pGO);
}


GameObject* GameObjectFactory::loadArchetype(std::string filePath)
{
	// Deserialize the given path.
	Serializer* pSer = GlobalManager::getSerializer();
	rapidjson::Document doc = pSer->loadJson(filePath);
	// Nullcheck the document.
	if (doc.IsNull() || !doc.IsObject())
	{
		// Game object creation failed.
		std::cout << "Error: Failed to load GameObject file " << filePath.c_str() << std::endl;
		return nullptr; // Return nullptr on fail.
	}

	// Pass to obj loadArchetype.
	return loadArchetype(doc.GetObject());
}

/// <summary>
/// Takes a file path as a string, loads the archetype found at that path,
/// then returns the resulting archetyped gameObject. 
/// DOES NOT PUSH ONTO GOM.
/// DOES NOT GENERATE A FULL-FLEDGED GAMEOBJECT.
/// </summary>
/// <param name="filePath"></param>
/// <returns>Archetyped GameObject or nullptr if failed</returns>
GameObject* GameObjectFactory::loadArchetype(rapidjson::GenericObject<false, rapidjson::Value> inputObj)
{
	// Create the new GameObject now that we know the json is reasonable.
	GameObject *pNewGO = new GameObject; 

	// Make sure components exists.
	if (!inputObj.HasMember("Components") || !inputObj["Components"].IsObject())
	{
		// No components to add to the game object. This is an empty game object.
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
				std::cout << "Warning: GameObject Archetype did not contain any components." << std::endl;
		return pNewGO; // Return empty GameObject since it was a valid GameObject but had no components.
	}

	// Parse through components.
	rapidjson::Value::ConstMemberIterator itr = inputObj["Components"].GetObject().MemberBegin();
	for (; itr != inputObj["Components"].GetObject().MemberEnd(); ++itr )
	{
		if (itr->name.IsString() == false)
		{
			std::cout << "Error: Failed to deserialize a component because. itr->name was not string." << std::endl;
			continue;
		}

		Component* pNewComponent = nullptr;
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
			std::cout << "GOF::Archetype - Reading in: " << itr->name.GetString() << "\n";
		pNewComponent = pNewGO->AddComponent(ComponentTypes::stringToEnum(itr->name.GetString()));

		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
			std::cout << "GOF::Archetype - Done Reading.\n";

		// Rapidjson didn't like me passing just the value in, so pass the whole iterator.
		if (nullptr != pNewComponent)
			pNewComponent->Serialize(itr);
	}


	// Set shader via archetype if specified.
	if (
		inputObj.HasMember("Shader")
		&& inputObj["Shader"].IsString()
		)
	{
		std::string newShaderName = inputObj["Shader"].GetString();
		pNewGO->mShaderName = newShaderName;
	}

	// Set render pass via archetype if its specified.
	if (
		inputObj.HasMember("Render Pass")
		&& inputObj["Render Pass"].IsString()
		)
		helper_objectRenderPass(pNewGO, inputObj["Render Pass"].GetString());
	// Remain NONE otherwise.

	// Simply return. Archetypes dont touch the game object manager.
	return pNewGO;
}


/// <summary>
/// Takes a rapidjson const genericObject 'inputObj' and parses it into a gameObject which is then 
/// pushed onto the game object manager.
/// Generated GameObject is also returned.
/// 
/// Note:
/// This is used by level loading. This should not be used mid-game.
/// </summary>
/// <param name="inputObj"></param>
/// <returns>Generated GameObject or nullptr if failed</returns>
GameObject* GameObjectFactory::loadObject(rapidjson::GenericObject<true, rapidjson::Value> inputObj)
{
	// Get the name
	std::string currentGOName;
	if (!inputObj.HasMember("Name") || !inputObj["Name"].IsString())
	{
		std::cout << "Error: GameObject in level failed to deserialize because it lacked a name or contained an invalid name." << std::endl;
		return nullptr;
	}

	currentGOName = inputObj["Name"].GetString();
	if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
		std::cout << "GOF::loadObject - Deserializing GameObject " << currentGOName.c_str() << std::endl;


	// Check if GO by this name aready exists...
	if (GlobalManager::getGameObjectManager()->mGameObjects.find(currentGOName) != GlobalManager::getGameObjectManager()->mGameObjects.end())
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
			std::cout << "Warning: GameObject by name " << currentGOName.c_str() << " already exists and will be replaced." << std::endl;
	}
	
	// Initialize as a nullptr.
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
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
			std::cout << "Warning: Archetype for GameObject was invalid or not present." << std::endl;
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
				if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
					std::cout << "Warning: Failed to parse a component while building level's GameObjects." << std::endl;
				continue;
			}

			if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
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
		pCurrentGO->setParent(parentName); // Will keep trying until it aquires the named gameobject.
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
	helper_initializeObject(pCurrentGO);
	
	return pCurrentGO;
}


/// <summary>
/// Used to dynamically create a new gameobject at game time. 
/// Takes a filepath (like an archetype) and takes a name.
/// 
/// Note: loadObject is used by level loading. THIS is used by things like creating indicators for bullets.
/// </summary>
/// <param name="filePath"></param>
/// <param name="gameObjectName"></param>
/// <returns></returns>
GameObject* GameObjectFactory::createDynamicGameObject(std::string filePath, std::string gameObjectName)
{
	// Use archetyping to create the initial gameobject.
	GameObject* pNewGO = loadArchetype(filePath);

	if (pNewGO == nullptr)
		return nullptr;

	// Set its name.
	pNewGO->mName = gameObjectName;

	// By default, set it to final render pass.
	pNewGO->setRenderPass(RenderPassType::FINAL);

	// Initialize onto the GOM.
	helper_initializeObject(pNewGO);

	return pNewGO;
}


GameObject* GameObjectFactory::generateProjectile(std::string fileName)
{
	return generateProjectile(fileName, "");
}

/// <summary>
/// May soon be depricated.
/// 
/// Generates a projectile according to the game object manager projectile counter.
/// 
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
GameObject* GameObjectFactory::generateProjectile(std::string fileName, std::string indicatorName)
{
	std::string filePath = GlobalManager::getResourceManager()->pathProjectiles + fileName;
	GameObject* pNewGO = loadArchetype(filePath);

	int projectileNumber = GlobalManager::getGameObjectManager()->getNextProjectile();
	std::string newGOName = "PROJECTILE_" + std::to_string(projectileNumber);

	pNewGO->mName = newGOName;

	if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
		std::cout << "GOF::Projectile - Deserializing GameObject '" << newGOName.c_str() <<"'" << std::endl;

	// I'm going to just assume all Dynamic GO's use final render pass.
	pNewGO->setRenderPass(RenderPassType::FINAL);

	// Automatically subscribe this to projectile events.
	GlobalManager::getEventManager()->Subscribe(EventType::DESTROY_PROJETILE, pNewGO);

	// Initialize and return.
	helper_initializeObject(pNewGO);

	// If indicator was specified, create that too.
	if (indicatorName.compare("") != 0)
	{
		filePath = GlobalManager::getResourceManager()->pathIndicators + indicatorName;
		GameObject* pNewIndicatorGO = loadArchetype(filePath);
		if (pNewIndicatorGO == nullptr)
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
				std::cout << "GOF::Projectile - Supplied indicator archetype " << filePath.c_str() << " failed to deserialize. Projectile will not have indicator." << std::endl;

			return pNewGO;
		}

		std::string newIndicatorGOName = newGOName + "_Indicator";

		pNewIndicatorGO->mName = newIndicatorGOName;

		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGOF)
			std::cout << "GOF::Projectile - Deserializing GameObject '" << newIndicatorGOName.c_str() << "'" << std::endl;

		pNewIndicatorGO->setRenderPass(RenderPassType::FINAL);

		helper_initializeObject(pNewIndicatorGO);

		pNewIndicatorGO->setParent(pNewGO->mName);
	}

	return pNewGO;
}

