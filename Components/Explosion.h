/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Explosion.h
	Purpose:			Handles the behavior for explosion game objects.
						Basically a non-corporeal expanding cloud of damage.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/8/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

#include "Transform.h"
#include "PhysicsBody.h"
#include "GLRect.h"


class Explosion : public Component
{
public:
	Explosion();
	~Explosion();

	void Initialize();
	void Update(); 

	float getIntensity() { return mIntensity; }
	void setIntensity(float intensity) // Sets start intensity too.
	{
		mIntensity = intensity;
		mStartIntensity = intensity;
	}

	void setUniformData(ShaderProgram* pProgram);

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	float mSize = 0.1f;

private:
	// No private methods.
private:
	float mIntensity = 100.0f;
	float mStartIntensity = 100.0f;
	Transform* mpTransform;
	PhysicsBody* mpPhysicsBody;
	GLRect* mpRect;
};

