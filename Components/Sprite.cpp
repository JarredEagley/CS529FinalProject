/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Sprite.cpp
	Purpose				Implements sprite component functions.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "Sprite.h"
#include "SDL_surface.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"
#include <iostream> 


Sprite::Sprite() : Component(ComponentTypes::TYPE_SPRITE)
{
	mpSurface = nullptr;
}

Sprite::~Sprite()
{
}

void Sprite::Update()
{
}
void Sprite::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check for serializing sprites.
	if (!inputMemberIt->value.IsString())
	{
		std::cerr << "Warning: Sprite failed to deserialize. Value was not a string." << std::endl;
		return;
	}

	//std::cout << "DEBUG: Passed " << inputMemberIt->name.GetString() << ", " << inputMemberIt->value.GetString() << " into sprite.cpp" << std::endl;
	std::string imagePath = ".\\Resources\\";
	const char* imageName = inputMemberIt->value.GetString();
	imagePath.append(imageName);
	//std::cout << "DEBUG: Sprite name is " << imagePath.c_str() << "\n";
	mpSurface = GlobalManager::getResourceManager()->loadSurface(imagePath.c_str());
} 