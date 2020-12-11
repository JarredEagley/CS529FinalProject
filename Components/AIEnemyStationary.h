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

#include <string>
#include "ShipData.h"
#include "PhysicsBody.h"

class AIEnemyStationary : public Component
{
public:
	AIEnemyStationary();
	~AIEnemyStationary();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram) {};
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj) {};

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// Navigation
	float mDesiredAltitude;
	
	// Agro range
	float mShootRange;
	float mMissileLaunchRange;

	// Missile launching
	float mMissileLaunchProbability;
	float mMissileLaunchTimerMax;

	std::string mOrientationBehvaior = ""; // Options are 'FACE_PLAYER' and 'FACE_PROGRADE'. If none specified, then AI will simply have no preferred orientation unless maneuvering.

private:
	void preferredOrientation();
	float alignToVector(glm::vec2 alignmentVector); // Returns alignment amount.

private:
	ShipData* mpShipData;
	PhysicsBody* mpPhysicsBody;
	
	float mMissileLaunchTimer;

	float mManeuveringSpeedThreshold = 10.0f;

	std::string mTargetName;

};

