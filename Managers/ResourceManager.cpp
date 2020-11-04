/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ResourceManager.cpp
	Purpose:			Contains the class methods for the resource manager.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "ResourceManager.h"
//#include "SDL_surface.h"
//#include "SDL_image.h"

#include "stb_image.h"

#include <iostream>

ResourceManager* ResourceManager::instance = nullptr;

void ResourceManager::destroySingleton()
{	
	// Nullcheck.
	if ( &mTextures != nullptr)
	{
		// Need to free everything in the hashmap!
		for (auto& keyValuePair : mTextures)
			delete keyValuePair.second; // TO-DO: Is this right?
			//SDL_FreeSurface(keyValuePair.second);  

		// Clear the hashmap too.
		mTextures.clear();
	}
	else
	{
		std::cout << "Waring: ResourceManager mSurfaces did not exist." << std::endl;
	}
	
	// Delete the singleton.
	delete instance;
}

ResourceManager::ResourceManager() {}

/*
SDL_Surface* ResourceManager::loadSurface(const char* pFilePath)
{
	// Try to fetch from the hash map.
	SDL_Surface* pSurface = mSurfaces[pFilePath];

	// If it already exists, return it!
	if (pSurface)
		return pSurface;

	// Else load it, store it, then return it.
	//pSurface = SDL_LoadBMP(pFilePath);
	pSurface = IMG_Load(pFilePath);

	// If successfully loaded, store it.
	if (pSurface)
		mSurfaces[pFilePath] = pSurface;

	return pSurface;
}
*/

stbi_uc* ResourceManager::loadTexture(const char* pFilePath)
{
	// Try to fetch from hash map.
	stbi_uc* pTex = mTextures[pFilePath];

	// If it already exists, return it!
	if (pTex)
		return pTex;

	// Else load it, store it, then return it.
	int width, height, nrChannels;
	pTex = stbi_load(pFilePath, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (pTex)
	{
		// Image loaded successfully, store it.
		mTextures[pFilePath] = pTex;
	}
	else
	{
		std::cerr << "Warning: Failed to load image: " << pFilePath << std::endl;
	}

	return pTex;
}