/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIEnemyStationary.h
	Purpose:			The most basic type of AI. If theres no gravity
						then it remains still. If there is gravity, it
						tries to maintain a specific orbit.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "ComponentTypes.h"

#include "AIEnemyCore.h"

class AIEnemyStationary : public Component
{
public:
	AIEnemyStationary();
	~AIEnemyStationary();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private functions. 
private:
	AIEnemyCore* mpAICore = nullptr;

};

