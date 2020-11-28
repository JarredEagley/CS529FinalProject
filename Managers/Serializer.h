/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Serializer.h
	Purpose:			Header file for the Serializer class. This only
						encapsulates json loading.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/24/2020

- End Header --------------------------------------------------------*/

#pragma once

// Couldn't find a good way to do forward declaration here.
#include "rapidjson/document.h"

class Serializer
{
public:
	static Serializer* getInstance()
	{
		if (!instance)
			instance = new Serializer;
		return instance;
	};
	void destroySingleton();

	rapidjson::Document loadJson(const char* pFileName); // For now, this just directly deserializes and returns.
	// void saveJson() {}; // Not going to do this.
public:
	// No public variables.
private:
	Serializer() {};
private:
	static Serializer* instance;
};

