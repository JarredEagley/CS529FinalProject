/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FollowCursor.h
	Purpose				Projects the cursor in screenspace to a position
						in worldspace. This is both broadcasted as an event
						and set as the component's owner game object's 
						transform position.
						NOTE:
						This is in a state of 'good enough for gameplay
						purposes' right now. It does not work perfectly.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/29/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Camera.h"

class FollowCursor : public Component
{
public:
	FollowCursor();
	~FollowCursor();

	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);
	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) {};

public:
	// No public variables.
private:
	// No private methods. 
private:
	Camera* mpCameraComponent;
	Transform* mpCameraTransformComponent;
	Transform* mpTransform;

};

