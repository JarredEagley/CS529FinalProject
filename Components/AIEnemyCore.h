/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIEnemyCore.h
	Purpose:			The core AI module which other types of AI contact
						for functions such as maintaining an orbit or navigating
						to a point in space.
						Also registers its gameObject as an AI which must
						be destroyed for player victory.
						Note:
						Does not implement any behvaior on its own. Simply
						a core for a behvior component.
						Essentially: Behvior tells Core what to do,
						core tells shipdata how to move.
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
#include "PhysicsBody.h"
#include "ShipData.h"

class AIEnemyCore : public Component
{
public:
	AIEnemyCore();
	~AIEnemyCore();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);


	// Helper functions.
	void getNearestGravityBody();

	void calculateOrbitalParameters();

	void keepOrbit();
	void matchVelocityVector(glm::vec2 desiredVelocity);
	void tryToStop();
	float alignToVector(glm::vec2 alignmentVector);
	void preferredOrientation();


	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// Data
	ShipData* mpShipData = nullptr;
	PhysicsBody* mpPhysicsBody = nullptr;

	// Gravity data
	PhysicsBody* mpNearestGravityBody = nullptr;
	bool isInGravity = false;
	glm::vec2 mNGB_RelativePosition = glm::vec2(0.0f);
	glm::vec2 mNGB_RelativePositionNormal = glm::vec2(0.0f);
	glm::vec2 mNGB_RelativeVelocity = glm::vec2(0.0f);
	float mNGB_DistSqr = 0.0f;
	float mNGB_Dist = 0.0f;

	// Orbital navigation data
	float mOrbitalSpeedAtCurrentAltitude = 0.0f;
	glm::vec2 mOrbitalVelocityPrograde = glm::vec2(0.0f); // Forward/backward
	glm::vec2 mOrbitalVelocityRadial = glm::vec2(0.0f); // Up/down
	float mNGB_RelativeSpeed = 0.0f;

	// General navigational data
	//float mSpeed = 0.0f; 

	// Default values for navigation parameters. AIStationary may override this.
	float mDesiredAltitude = 5000.0f;
	float mManeuveringSpeedThreshold = 10.0f;
	float mOrbitalAdjustmentAgression = 0.2f;
	float mOrbitThickness = 50.0f; // Fudge factor for desired orbit so its not ALWAYS trying to maneuver.
	std::string mOrientationBehavior = "";

	// Serialized Agro parameters
	float mShootRange = 0.0f;
	float mMissileLaunchRange = 0.0f;

	float mMissileLaunchProbability = 0.0f;
	float mMissileLaunchTimer = 0.0f;

private:
	// No private methods.
private:
	float mMissileLaunchTimerMax = 100.0f;

};

