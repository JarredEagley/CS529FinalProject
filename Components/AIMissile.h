/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIMissile.h
	Purpose				AI Behavior for missiles. Will work practically
						identical to my missiles in Asteroids. These will
						be a decently effective but very finite long-range
						attack.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

#include "ShipData.h"
#include "PhysicsBody.h"
#include "GLRect.h"

class AIMissile : public Component
{
public:
	AIMissile();
	~AIMissile();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram) {};
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj) {};

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	std::string mTargetName; // Target. If missile loses its target, it will die.
	float mDetonateDistance; // Missiles will pretty much never hit directly, so they explode a set distance from their target to wash the target with the explosion.
	float mWarheadIntensity; // Intensity of this missile's warhead.
	float mOrthoVelocityCorrection;

private:
	// No private methods.
private:
	float mActivateTimer; // Waits a given amount of time before activating.
	float mInactiveLifespan;
	ShipData* mpShipData = nullptr;
	PhysicsBody* mpPhysicsBody = nullptr;
	GLRect* mpGLRect = nullptr;

};

