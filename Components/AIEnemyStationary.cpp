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
#include "glm/gtx/projection.hpp"

AIEnemyStationary::AIEnemyStationary() : Component(ComponentTypes::TYPE_AI_STATIONARY),
mpPhysicsBody(nullptr), mpShipData(nullptr),
mDesiredAltitude(7000.0f),
mShootRange(4000.0f), mMissileLaunchRange(5000.0f),
mMissileLaunchProbability(0.5f), mMissileLaunchTimer(0.0f), mMissileLaunchTimerMax(2000.0f)
{
}

AIEnemyStationary::~AIEnemyStationary()
{
	// Remove from the enemies list.
	GlobalManager::getGameStateManager()->mLivingEnemies.remove(this->mpOwner->mName);
}


void AIEnemyStationary::Initialize()
{
	// Register with the enemies list.
	GlobalManager::getGameStateManager()->mLivingEnemies.push_back(this->mpOwner->mName);
}


void AIEnemyStationary::Update()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;

	if (mpShipData == nullptr || mpPhysicsBody == nullptr)
	{
		mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
		mpPhysicsBody = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		return;
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

void AIEnemyStationary::keepOrbit(float closestDistSqr, PhysicsBody* pClosest)
{
	// Gravity
	float const G_CONST = GlobalManager::getPhysicsManager()->universalGravitationalConstant;

	// Unfortunately, orbital mechanics involves quite a few sqrts sometimes.
	float orbitalRadius = sqrt(closestDistSqr);
	float orbitalSpeedAtCurrentAltitude = sqrt((G_CONST * pClosest->mMass) / (orbitalRadius * 1000.0f));
	orbitalSpeedAtCurrentAltitude /= 1000.0f;

	//std::cout << "orbital speed = " << orbitalSpeedAtCurrentAltitude << std::endl;
	//std::cout << "current speed = " << glm::length(mpPhysicsBody->mVelocity) << std::endl;

	glm::vec2 relativeVelocity = mpPhysicsBody->mVelocity - pClosest->mVelocity;
	glm::vec2 relativePosition = mpPhysicsBody->mPosition - pClosest->mPosition;
	glm::vec2 relativePositionNormal = glm::vec2(relativePosition.y, -relativePosition.x);

	// More unfortuantely expensive calculations.
	glm::vec2 progradeVelocity = glm::proj(relativeVelocity, relativePositionNormal);	// forward/backwards
	glm::vec2 radialVelocity = glm::proj(relativeVelocity, relativePosition);			// up/down


	//std::cout << "Prograde:" << progradeVelocity.x << ", " << progradeVelocity.y << std::endl;
	//std::cout << "Radial:" << radialVelocity.x << ", " << radialVelocity.y << std::endl;

	//std::cout << "Orbital radius is " << orbitalRadius << " and desired altitiude is " << mDesiredAltitude << std::endl;

	// Go up or down.
	if (orbitalRadius > mDesiredAltitude)
	{
		// Try go down
		// Cut our orbital velocity down a little.
		float desiredSpeed = orbitalSpeedAtCurrentAltitude * (1.0f-mOrbitalAdjustmentAgression);
		// Aim our velocity vector down a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(relativePosition) * (0.0f-mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(progradeVelocity) * orbitalSpeedAtCurrentAltitude * (1.0f-mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		matchVelocityVector(desiredVelocityVector + pClosest->mVelocity);
	}
	else
	{
		// Try go up
		// Increase our velocity a little
		float desiredSpeed = orbitalSpeedAtCurrentAltitude * (1.0f+mOrbitalAdjustmentAgression);
		// Aim our velocity vector up a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(relativePosition) * (0.0f+mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(progradeVelocity) * orbitalSpeedAtCurrentAltitude * (1.0f+mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		matchVelocityVector(desiredVelocityVector + pClosest->mVelocity);
	}
}


void AIEnemyStationary::matchVelocityVector(glm::vec2 desiredVelocity)
{
	glm::vec2 currentVelocity = mpPhysicsBody->mVelocity;
	float speedDiff = glm::distance( desiredVelocity, currentVelocity);
	glm::vec2 velocityDiff = desiredVelocity - currentVelocity ;
	glm::vec2 velocityDiffNormalized = glm::normalize(velocityDiff);

	//std::cout << "Desired speed is " << glm::length(desiredVelocity) << std::endl;
	//std::cout << "Speed diff: " << speedDiff << std::endl;

	// Use drive if speed above a threshold and fuel not depleted.
	if (speedDiff > mManeuveringSpeedThreshold && mpShipData->mFuel > 0.1f)
	{
		// Align
		float alignmentAmount = alignToVector(-velocityDiffNormalized);

		//std::cout << "Throttle " << (1.0f / abs(alignmentAmount) - 1.0f) * 1.5f << std::endl;

		// Accelerate
		if (alignmentAmount == 0)
			mpShipData->setThrottle(100.0f);
		else
			mpShipData->setThrottle((1.0f / abs(alignmentAmount) - 1.0f) * 1.5f);
	}
	else
	{
		// Do orientation lock.
		preferredOrientation();
	}
	// Use maneuvering thrusters regardless.
	mpShipData->applyThrustSecondary(velocityDiffNormalized);

}

float AIEnemyStationary::alignToVector(glm::vec2 alignmentVector)
{
	glm::vec2 myNormal = mpPhysicsBody->mRightDir;
	float alignmentAmount = glm::dot(alignmentVector, myNormal);

	// Turning +
	if (alignmentAmount > 0)
	{
		// Alignment amount = desired turn speed.
		if (mpPhysicsBody->mAngularVelocity > alignmentAmount)
			mpShipData->applySpin(-alignmentAmount);
		else
			mpShipData->applySpin(alignmentAmount);
	}
	// Turning -
	else
	{
		// Alignment amount = desired turn speed.
		if (mpPhysicsBody->mAngularVelocity < alignmentAmount)
			mpShipData->applySpin(-alignmentAmount);
		else
			mpShipData->applySpin(alignmentAmount);
	}

	return alignmentAmount;
}

void AIEnemyStationary::preferredOrientation()
{
	if (mOrientationBehvaior == "FACE_PLAYER")
	{

	}
	else if (mOrientationBehvaior == "FACE_PROGRADE")
	{

	}
	// By default, no need to do anything else; just use the natural angular damping specified in ship data.
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