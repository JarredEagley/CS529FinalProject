/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Sprite.h
	Purpose				Header file for the sprite component, which handles
						a GameObject's appearance (its sprite).
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "rapidjson/document.h"

class SDL_Surface;
//class GameObject; // Removing this seems to not cause problems. I think that's due to linking order.

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();

	void Update();
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	SDL_Surface* mpSurface; // Actual surface is stored in the resource manager.
private:
	// No private methods.
private:
	// No private variables.
};

