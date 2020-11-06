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
std::unordered_map<const char*, GLuint> ResourceManager::mTextures;

void ResourceManager::destroySingleton()
{	
	// TO-DO: Strings are char*, do I need to delete them?

	// Clear the hashmap.
	mTextures.clear();
	
	// Delete the singleton.
	delete instance;
}

ResourceManager::ResourceManager() 
{}


GLuint ResourceManager::loadTexture(const char* texName)
{
	// Test if the texture by the given name exists. If it does, return it.
	if (mTextures.find(texName) != mTextures.end())
		return mTextures[texName];

	// Texture by that name doesn't exist. Load it using stb_image, store in vram, then store its id.
	//std::string texPath = pathTextures + texName; // TO-DO: At what level do I turn the name into a path? Needs to be consistant.

	int width, height, nrChannels;
	//stbi_uc* data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha); 
	stbi_uc* data = stbi_load(texName, &width, &height, &nrChannels, STBI_rgb_alpha);
	GLuint texId = NULL;
	if (data != nullptr)
	{
		// Generate the texId and return it.
		// Generate and bind.
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Store the data.
		
		// 10 mipmap levels.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10); 
		glGenerateMipmap(GL_TEXTURE_2D);

		// Some parameters needed for textures.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		//std::cout << "Warning: Failed to load image: " << texPath << std::endl;
		std::cout << "Warning: Failed to load image: " << texName << std::endl;
	}
	// Clear data.
	stbi_image_free(data);


	// Returns NULL if failed, the generated id if success.
	return texId;
}
