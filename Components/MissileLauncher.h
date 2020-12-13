/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			MissileLauncher.h
	Purpose				A component for a missile launcher, which acts
						roughly similar to turrets. When the player is locked
						onto a target they may press the missile hotkey
						to fire a guided missile which will rapidly 
						accelerate toward the target and explode.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

#include <string>
#include "GLRect.h"

class MissileLauncher : public Component
{
public:
	MissileLauncher();
	~MissileLauncher();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private methods.
private:
	std::string mTargetGOName = "";
	GLRect* mpGLRect = nullptr;

	// Serialized:
	glm::vec2 mLaunchForce;

};

