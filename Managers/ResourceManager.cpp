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
#include <iostream>
//#include "SDL_surface.h"
//#include "SDL_image.h"

#include "stb_image.h"

ResourceManager* ResourceManager::instance = nullptr;

void ResourceManager::destroySingleton()
{	
	// Nullcheck.
	if ( !mTextures.empty() ) // TO-DO: This is not working >:(
	{
		// Need to free everything in the hashmap!
		for (auto keyValuePair : mTextures)
		{
			stbi_image_free(keyValuePair.second->texture);
		}
			//SDL_FreeSurface(keyValuePair.second);  

		// Clear the hashmap too.
		mTextures.clear();
	}
	else
	{
		std::cout << "Waring: ResourceManager mTextures did not exist." << std::endl;
	}
	
	
	// Delete the singleton.
	delete instance;
}

ResourceManager::ResourceManager() 
{
	//mTextures = std::unordered_map<std::string, stbiTexture >();
}

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

stbiTexture* ResourceManager::loadTexture(const char* pFilePath)
{
	// Try to fetch from hash map.
	//stbi_uc* pTex = mTextures[pFilePath].texture; // TO-DO: I'm sleepy, may be missing some nullchecks here...

	//stbiTexture &tex = mTextures;
	
	// If it already exists, return it!
	if (mTextures.find(pFilePath) != mTextures.end())
		return mTextures[pFilePath];

	// Else load it, store it, then return it.
	//int width, height, nrChannels;
	//pTex = stbi_load(pFilePath, &mTextures[pFilePath].width, &mTextures[pFilePath].height, &mTextures[pFilePath].nrChannels, STBI_rgb_alpha);
	stbi_uc* data = stbi_load(pFilePath, &mTextures[pFilePath]->width, &mTextures[pFilePath]->height, &mTextures[pFilePath]->nrChannels, STBI_rgb_alpha);
	if (data != nullptr)
	{
		// Image loaded successfully, store it.
		mTextures[pFilePath]->texture = data; // TO-DO: I think I"m getting context issues here.....
		//mTextures[pFilePath].texture = ; // TO-DO: I think I"m getting context issues here.....
	}
	else
	{
		std::cerr << "Warning: Failed to load image: " << pFilePath << std::endl;
	}

	return mTextures[pFilePath];
}