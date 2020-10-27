/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ResourceManager.h
	Purpose:			Defines the resource manager class.
						Now a singleton.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once
#include <unordered_map>
//#include "SDL_surface.h" We can use forward declaration to avoid including this!

class SDL_Surface; // All we're doing is letting this class know another class by this name exist. Reduces code size and build time.

class ResourceManager
{
public:
	static ResourceManager* getInstance()
	{
		if (!instance)
			instance = new ResourceManager;
		return instance;
	};
	void destroySingleton();

	SDL_Surface* LoadSurface(const char *pFilePath);
public:
	// No public variables.
private:
	ResourceManager();
private:
	static ResourceManager* instance;
	std::unordered_map<std::string, SDL_Surface* > mSurfaces; // Hash map.
};