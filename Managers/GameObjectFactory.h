/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameObjectFactory.h
	Purpose:			The header file for my engine's GameObject Factory.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/23/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>

class GameObject;

class GameObjectFactory 
{
public:
	static GameObjectFactory* getInstance()
	{
		if (!instance)
		{
			instance = new GameObjectFactory();
		}
		return instance;
	};
	void destroySingleton();

	GameObject* loadArchetype(std::string pFileName); // Returns a GameObject pointer matching the given archetype.
	GameObject* loadObject(rapidjson::GenericObject<true, rapidjson::Value> inputObj);

	// Creates a GameObject from a rapidjson GenericObject. Archetypes will be created recursively.
	// Will return the name of the created GO.
	std::string loadGameObject(rapidjson::GenericObject<false, rapidjson::Value> inputObj, std::string namePre, std::string namePost);
	std::string loadGameObject(rapidjson::GenericObject<false, rapidjson::Value> inputObj);

	// Will return the string header at the top of the list, OR ""
	std::string loadGameObjectList(rapidjson::GenericArray<false, rapidjson::Value> inputArr,
		std::string namePre, std::string namePost);
	std::string loadGameObjectList(rapidjson::GenericArray<false, rapidjson::Value> inputArr);

	// Will generate a new game object using the designated filename.
	GameObject* generateProjectile(std::string pFileName);

public:
	// No public variables.
private:
	GameObjectFactory();

	void helper_objectRenderPass(GameObject* pGO, const char* renderPassType);

private:
	static GameObjectFactory *instance;
};

