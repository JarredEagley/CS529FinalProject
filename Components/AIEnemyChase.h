/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIEnemyChase.h
	Purpose:			A more complex AI which will chase down the player.
						Once within a certain range, it will try to match
						velocity with the player.
						Within a certain range, it will essentially
						act like the missile AI.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/10/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "ComponentTypes.h"

#include "AIEnemyCore.h"

class AIEnemyChase : public Component
{
public:
	AIEnemyChase();
	~AIEnemyChase();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// Below this distance, even in gravity it will try to move relative to the player.
	float bruteForceDistance = 1000.0f;
	// Minimum distance AI is willing to get to the player.
	float minimumDistance = 500.0f;

private:
	// No private functions. 
private:
	AIEnemyCore* mpAICore = nullptr;

};

