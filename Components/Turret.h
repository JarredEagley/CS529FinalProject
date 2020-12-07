/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Turret.h
	Purpose				Handles the behavior for a coilgun turret. This
						is supplied by some means a direction to aim
						and a boolean to shoot. This means both players
						and AI can command them.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/29/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "../Managers/GlobalManager.h"

class Transform;
class GLRect;

class Turret : public Component
{
public:
	Turret();
	~Turret();

	void Initialize();
	void Update(); 

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
	void handleEvent(Event* pEvent);

public:
	glm::vec2 mAimPoint;

private:
	// No private methods.
private:
	float mAimAngle;
	float fireRate = 10; // TO-DO: Serialize
	float fireTimer = 10; // TO-DO: Serialize
	float mProjectileVelocity = 100; // TO-DO: Serialize
	bool mIsShooting;
	Transform* mpTransform;
	Transform* mpParentTransform;
	GLRect* mpGLRect;
	GLRect* mpParentGLRect;

};

