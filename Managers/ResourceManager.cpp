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
{}


int ResourceManager::loadTexture(const char* texName)
{
	// Test if the texture by the given name exists. If it does, return it.
	if (mTextures.find(texName) == mTextures.end())
		return mTextures[texName];

	// Texture by that name doesn't exist. Load it using stb_image, store in vram, then store its id.
	std::string texPath = pathTextures + texName;

	int width, height, nrChannels;
	stbi_uc* data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data != nullptr)
	{
		// Generate the texId and return it.
		int texId;
		glGenTextures();
	}
	else
	{
		// NULL will be returned.
		std::cout << "Warning: Failed to load image: " << texPath << std::endl;
	}
}
