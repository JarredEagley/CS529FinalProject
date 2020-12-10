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

#include "AIMissile.h"
#include "glm/gtx/projection.hpp"
#include "../Generators.h"

AIMissile::AIMissile() : Component(ComponentTypes::TYPE_UNDEFINED),
mActivateTimer(1000.0f), mTargetName(""), mWarheadIntensity(1000.0f), mDetonateDistance(100.0f),
mInactiveLifespan(100000.0f),
mOrthoVelocityCorrection(2.0f)
{
}

AIMissile::~AIMissile()
{
}


void AIMissile::Initialize()
{
}

void AIMissile::Update()
{
	if (mpShipData == nullptr || mpPhysicsBody == nullptr)
	{
		mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
		mpPhysicsBody = static_cast< PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		return;
	}

	if (mInactiveLifespan < 0.0f)
	{
		// Kill this missile.
		// TO-DO
		this->mpOwner->mIsMarkedForDelete = true;
	}

	if (mActivateTimer < 0.0f)
	{
		// --- Do seeking logic. --- //

		// Try to find our target.
		GameObject* pTargetGO = GlobalManager::getGameObjectManager()->getGameObject(this->mTargetName);
		if (pTargetGO == nullptr)
		{
			// Tick down the amount of time its allowed to stay idle and alive.
			mInactiveLifespan -= GlobalManager::getPhysicsManager()->getGameTime();
			return;
		}
		PhysicsBody* pTargetPhysics = static_cast<PhysicsBody*>(pTargetGO->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		if (pTargetPhysics == nullptr)
		{
			// Tick down the amount of time its allowed to stay idle and alive.
			mInactiveLifespan -= GlobalManager::getPhysicsManager()->getGameTime();
			return;
		}

		// Past here, we have our target. Check fuel level.
		if (mpShipData->mFuel <= 1.0f)
		{
			// Tick down the amount of time its allowed to stay idle and alive.
			mInactiveLifespan -= GlobalManager::getPhysicsManager()->getGameTime();
			// Don't return, we're allowed to drift without fuel toward the enemy.
		}

		// Used in both homing and detonation behavior.
		glm::vec2 targetRelativePosition = pTargetPhysics->mPosition - mpPhysicsBody->mPosition;
		float targetDistance = glm::length(targetRelativePosition);

		// Test for detonation.
		if (targetDistance < mDetonateDistance)
		{
			// Go kaboom!
			generateExplosion(mpPhysicsBody, mWarheadIntensity, "Explosion_NoDebris.png");
			this->mpOwner->mIsMarkedForDelete = true;
		}

		// Homing behavior.
		
		 glm::vec2 velocityDifference = pTargetPhysics->mVelocity - mpPhysicsBody->mVelocity;
		
		glm::vec2 alignmentVec = mpPhysicsBody->mForwardDir;
		glm::vec2 normalVec = mpPhysicsBody->mRightDir;

		glm::vec2 targetRelativePositionNormal = glm::vec2(targetRelativePosition.y, -targetRelativePosition.x);

		// The most important part!
		glm::vec2 orthoVelocity = glm::proj(velocityDifference, targetRelativePositionNormal);

		orthoVelocity *= mOrthoVelocityCorrection;

		// Finally, the guidance vector.
		glm::vec2 guidanceVector = targetRelativePosition + orthoVelocity;

		guidanceVector *= 5.0f; // Should probably make this data driven.

		// Figure out our desired throttle.
		float alignmentAmount = glm::dot(guidanceVector, normalVec);
		float desiredThrottle;
		if (alignmentAmount >= 0)
			desiredThrottle = 30.0f / ((alignmentAmount / 10.0f) + 1.0f);
		else
			desiredThrottle = 30.0f / ((-alignmentAmount / 10.0f) + 1.0f);

		// Dot product to get target angle, point at target.
		float targetAngle = glm::dot(normalVec, guidanceVector);
		if (targetAngle > 0)
		{
			// Might need to give these a bit of PID. hopefully not.
			mpShipData->applySpin(1.0f);
		}
		else
		{
			mpShipData->applySpin(-1.0f);
		}

		mpShipData->setThrottle(desiredThrottle);



		// TO-DO: Send target a missile lockon event to let them know to shoot at or evade the missile.

	}
	else
		mActivateTimer -= GlobalManager::getPhysicsManager()->getGameTime();
}

void AIMissile::handleEvent(Event* pEvent)
{
}


void AIMissile::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Detonation Distance") && inputObj["Detonation Distance"].IsNumber())
	{
		this->mDetonateDistance = inputObj["Detonation Distance"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Missile component did not find a detonation distance parameter. Default used." << std::endl;


	if (inputObj.HasMember("Warhead Intensity") && inputObj["Warhead Intensity"].IsNumber())
	{
		this->mDetonateDistance = inputObj["Warhead Intensity"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Missile component did not find a warhead intensity parameter. Default used." << std::endl;


	if (inputObj.HasMember("Inactive Lifespan") && inputObj["Inactive Lifespan"].IsNumber())
	{
		this->mDetonateDistance = inputObj["Inactive Lifespan"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Missile component did not find a Inactive Lifespan parameter. Default used." << std::endl;


	if (inputObj.HasMember("Orthogonal Velocity Correction") && inputObj["Orthogonal Velocity Correction"].IsNumber())
	{
		this->mDetonateDistance = inputObj["Orthogonal Velocity Correction"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Missile component did not find a Orthogonal Velocity Correction parameter. Default used." << std::endl;


}