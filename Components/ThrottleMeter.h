/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ThrottleMeter.h
	Purpose				A component for the player's throttle meter.
						Note:
						This is not the throttle backdrop, but rather the
						indicator icon that slides up and down.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/26/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

class Transform;

class ThrottleMeter : public Component
{
public:
	ThrottleMeter();
	~ThrottleMeter();

	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);
	
	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	Transform* mpTransform;
	Transform* mpParentTransform;
};

