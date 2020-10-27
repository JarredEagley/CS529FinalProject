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

class GameObject;

class GameObjectFactory // TO-DO: Eventually make this a singleton probably, and move it into Managers directory... 
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GameObject* loadObject(const char* pFileName); // Returns a GameObject pointer matching the given archetype.
	void loadLevel(const char* pFileName);
public:
private:
private:
};

