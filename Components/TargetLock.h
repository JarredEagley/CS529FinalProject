/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			TargetLock.h
	Purpose:			Copies the transform data of the player's locked
						target to display a visual indicator over it.
						If no target is present (dead or missing) then
						snaps to 0,0 and turns invisible.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

class TargetLock : public Component
{
public:
	TargetLock();
	~TargetLock();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private methods.
private:
	// No private variables.
};

