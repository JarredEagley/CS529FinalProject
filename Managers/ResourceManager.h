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
#include "stb_image.h"

#include "GL/glew.h" 

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

	enum TexType
	{
		REPEAT,
		EXTEND,
		CLIP
	};

	GLuint loadTexture(const char* texName); // Returns the id of the texture loaded (or cached) (Default: Wrap texture.)
	GLuint loadTexture(const char* texName, TexType texType); // Returns the id of the texture loaded (or cached)

	void loadLevel(const char* pFileName);

public:
	const std::string pathResources  = ".\\Resources\\"; // The base-folder for storing resources like configs and textures.
	const std::string pathTextures	 = pathResources + "Textures\\";
	const std::string pathLevels	 = pathResources + "Levels\\";
	const std::string pathArchetypes = pathResources + "Archetypes\\";
	const std::string pathConfig     = pathResources + "Config.json";
	std::string currentLevel;

private:
	ResourceManager();
private:
	static ResourceManager* instance;
	static std::unordered_map<const char*, GLuint> mTextures; // textureName, texId
};