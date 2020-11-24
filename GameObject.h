/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameObject.h
	Purpose:			The header file for GameObject.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once

#include <unordered_map>

class Component;
class Event;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	Component* AddComponent(unsigned int Type); // Returns the component which was allocated.
	Component* GetComponent(unsigned int Type); // Because we need to access components. 
	void handleEvent(Event *pEvent);
public:
	std::unordered_map<unsigned int, Component*> mComponents; // TYPE, COMPONENT
	int getID() { return this->gameObjectID; }
	const char* shaderName = "core";	// May have more shaders in the future.
private:
	// No private methods.
private:
	static unsigned int maxId; // Keeping track of how many GameObjects we've made. // NOT CURRENTLY USED AT ALL
	int gameObjectID; 
};

