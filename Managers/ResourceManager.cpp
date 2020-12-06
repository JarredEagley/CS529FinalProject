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
#include "GlobalManager.h"
#include <iostream>
#include <fstream>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "stb_image.h"

ResourceManager* ResourceManager::instance = nullptr;
std::unordered_map<const char*, GLuint> ResourceManager::mTextures;
std::map<char, Character> ResourceManager::mCharacters;

void ResourceManager::destroySingleton()
{	
	// Clear the hashmap.
	mTextures.clear();
	
	// Delete the singleton.
	delete instance;
}

ResourceManager::ResourceManager() 
{
	stbi_set_flip_vertically_on_load(true);
	initializeCharacterMap();
}

GLuint ResourceManager::loadTexture(const char* texName)
{
	return loadTexture(texName, TexType::REPEAT);
}

GLuint ResourceManager::loadTexture(const char* texName, TexType _texType)
{
	// Test if the texture by the given name exists. If it does, return it.
	if (mTextures.find(texName) != mTextures.end())
		return mTextures[texName];

	// Texture by that name doesn't exist. Load it using stb_image, store in vram, then store its id.

	int width, height, nrChannels;
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

		// Some parameters needed for textures

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		switch (_texType)
		{
		case TexType::REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TexType::EXTEND:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case TexType::CLIP:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // I'm not positive this is right...
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;
		default:
			// Default behavior is just repeat.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		}

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


void ResourceManager::loadLevel(const char* pFileName)
{
	// Use serializer to read the json in.
	//std::ifstream inputStream(pFileName);
	Serializer* pSer = GlobalManager::getSerializer();
	std::string filePath = pathLevels + pFileName;
	rapidjson::Document doc = pSer->loadJson(filePath.c_str());

	// Nullcheck.
	if (doc.IsNull())
	{
		std::cerr << "Error: Failed to load level with filename " << pFileName << std::endl;
		return;
	}

	// Make sure this level has GameObjects to parse.
	// Note: GameObjects member is an array/list of documents.
	if (!doc.HasMember("GameObjects") || !doc["GameObjects"].IsArray())
	{
		std::cerr << "Warning: File " << pFileName << " did not contain any GameObjects." << std::endl;
		return;
	}

	// Loop through the GameObjects array.
	for (rapidjson::Value::ConstValueIterator arrItr = doc["GameObjects"].GetArray().Begin();
		arrItr != doc["GameObjects"].GetArray().End();
		++arrItr)
	{
		// Make sure this arrItr is an object.
		if (!arrItr->IsObject())
		{
			std::cout << "Warning: Failed to parse an object while loading level." << std::endl;
			continue;
		}

		// Load the object.
		GlobalManager::getGameObjectFactory()->loadObject(arrItr->GetObject());
	}
}


void ResourceManager::initializeCharacterMap()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Error::FreeType: Could not initialize FreeType Library." << std::endl;
		return;
	}

	FT_Face face;
	std::string font = pathFont + "arial.ttf";
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		std::cout << "Error::FreeType: Failed to load font '" << font << "'." << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Not sure if this'll mess up my existing texture loading... 
	for (unsigned char c = 0; c < 128; ++c)
	{
		// Load the glyph.
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Error::FreeType: Failed to load glyph '" << c << "'." << std::endl;
			continue;
		}

		// Generate texture.
		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store character in the map to use later.
		Character character = {
			texId,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
		};
		mCharacters.insert(std::pair<char, Character>(c, character));
	}
}
