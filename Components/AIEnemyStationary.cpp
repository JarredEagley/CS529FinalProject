/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIEnemyStationary.cpp
	Purpose:			Implementations for AIEnemyStationary.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#include "AIEnemyStationary.h"
#include "../Managers/GlobalManager.h"


AIEnemyStationary::AIEnemyStationary() : Component(ComponentTypes::TYPE_AI_STATIONARY)
{
}

AIEnemyStationary::~AIEnemyStationary()
{
}


void AIEnemyStationary::Initialize()
{
}


void AIEnemyStationary::Update()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;

	if (mpAICore == nullptr)
	{
		mpAICore = static_cast<AIEnemyCore*>(mpOwner->GetComponent(ComponentTypes::));
	}


	// --- behavior --- //

	// Find the closest gravity body.
	glm::vec2 myPosition = mpPhysicsBody->mPosition;
	PhysicsBody *pClosest = nullptr;
	float closestDistSqr = -1.0f;
	for (auto pGBody : GlobalManager::getPhysicsManager()->gravityBodies)
	{
		glm::vec2 thisBodyPos = pGBody->mPosition;
		float thisDistSqr = (thisBodyPos.x-myPosition.x)*(thisBodyPos.x - myPosition.x)
			+ (thisBodyPos.y - myPosition.y)*(thisBodyPos.y - myPosition.y);

		if (closestDistSqr == -1.0f || closestDistSqr > thisDistSqr)
		{
			pClosest = pGBody;
			closestDistSqr = thisDistSqr;
		}
	}

	
	if (pClosest == nullptr)
	{
		// No gravity. Fight velocity.
		glm::vec2 myVelocity = mpPhysicsBody->mVelocity;
		
		glm::vec2 accelVector = glm::normalize(myVelocity);
		glm::vec2 accelNormal = glm::vec2(accelVector.y, -accelVector.x);

		float mySpeed = glm::length(myVelocity);

		// Use drive if speed above a threshold and fuel not depleted.
		if (mySpeed > mManeuveringSpeedThreshold && mpShipData->mFuel > 0.1f)
		{
			// Align
			float alignmentAmount = alignToVector(accelVector);

			// Accelerate
			if (alignmentAmount == 0)
				mpShipData->setThrottle(100.0f);
			else
				mpShipData->setThrottle((1.0f / abs(alignmentAmount) - 1.0f)*1.5f );
		}
		else
		{
			// Do orientation lock.
			preferredOrientation();
		}
		
		// Use maneuvering thrusters too regardless.
		mpShipData->applyThrustSecondary(-accelVector);
	}
	else
	{
		keepOrbit(closestDistSqr, pClosest);
	}
}




void AIEnemyStationary::handleEvent(Event* pEvent)
{
}


void AIEnemyStationary::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Desired Altitude") && inputObj["Desired Altitude"].IsNumber())
	{
		this->mDesiredAltitude = inputObj["Desired Altitude"].GetFloat();
	}

	if (inputObj.HasMember("Shoot Range") && inputObj["Shoot Range"].IsNumber())
	{
		this->mShootRange = inputObj["Shoot Range"].GetFloat();
	}

	if (inputObj.HasMember("Missile Launch Range") && inputObj["Missile Launch Range"].IsNumber())
	{
		this->mMissileLaunchRange = inputObj["Missile Launch Range"].GetFloat();
	}

	if (inputObj.HasMember("Missile Launch Probability") && inputObj["Missile Launch Probability"].IsNumber())
	{
		this->mMissileLaunchProbability = inputObj["Missile Launch Probability"].GetFloat();
	}

	if (inputObj.HasMember("Missile Launch Time") && inputObj["Missile Launch Time"].IsNumber())
	{
		this->mMissileLaunchTimerMax = inputObj["Missile Launch Time"].GetFloat();
		this->mMissileLaunchTimer = mMissileLaunchTimerMax;
	}


	if (inputObj.HasMember("Orientation Behavior") && inputObj["Orientation Behavior"].IsString())
	{
		this->mOrientationBehvaior = inputObj["Orientation Behavior"].GetString();
	}


	if (inputObj.HasMember("Maneuvering Speed Threshold") && inputObj["Maneuvering Speed Threshold"].IsNumber())
	{
		this->mManeuveringSpeedThreshold = inputObj["Maneuvering Speed Threshold"].GetFloat();
	}


	if (inputObj.HasMember("Orbital Adjustment Agression") && inputObj["Orbital Adjustment Agression"].IsNumber())
	{
		this->mOrbitalAdjustmentAgression = inputObj["Orbital Adjustment Agression"].GetFloat();
	}



}