/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Turret.cpp
	Purpose				Implementations for Turret.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/29/2020

- End Header --------------------------------------------------------*/

#include "Turret.h"

#include "ComponentTypes.h"
#include "Transform.h"
#include "GLRect.h"

#include "PhysicsBody.h"
#include <random>

Turret::Turret() : Component(ComponentTypes::TYPE_TURRET),
mpTransform(nullptr), mpParentTransform(nullptr),mpGLRect(nullptr), mpParentGLRect(nullptr),
mAimPoint(glm::vec2(0.0f)), mAimAngle(0.0f),
mIsShooting(false)
{
	// Randomize this turret's fire timer start point.
	float randomFloat = GlobalManager::getGameStateManager()->getRandomFloat();
	this->fireTimer = (randomFloat / RAND_MAX) * fireRate;
}

Turret::~Turret()
{
}


void Turret::Initialize()
{
	GlobalManager::getEventManager()->Subscribe(EventType::TRANSFORM_UPDATED, mpOwner);
	GlobalManager::getEventManager()->Subscribe(EventType::TURRET_COMMAND, mpOwner);
}

void Turret::Update()
{
	if (!mIsShooting)
		return;

	if (fireTimer > fireRate)
	{
		GameObject* pBullet = GlobalManager::getGameObjectFactory()->generateProjectile("CoilBullet.json");
		if (pBullet == nullptr)
			return;

		PhysicsBody* pBulletPhys = static_cast<PhysicsBody*>(pBullet->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		PhysicsBody* pParentPhys = static_cast<PhysicsBody*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		Transform* pBulletTransform = static_cast<Transform*>(pBullet->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		if (pBulletPhys != nullptr && pBulletTransform != nullptr && pParentPhys != nullptr)
		{
			// Ignore our parent's physics body for one second. 
			pBulletPhys->setTimedIgnoreCollision(pParentPhys, 1000.0f);

			// Set trasnform
			pBulletTransform->setPosition(mpParentTransform->getPosition() + mpTransform->getPosition());
			pBulletTransform->setRotation(mAimAngle);

			// Set physics
			pBulletPhys->mVelocity = pParentPhys->mVelocity; // inherit velocity.
			glm::vec2 fireVec = mAimPoint-glm::vec2(mpParentTransform->getPosition());
			fireVec = glm::normalize(fireVec) * 5.0f; // Arbitrary velocity
			pBulletPhys->applyForce(fireVec);
		}
		
		fireTimer = 0;
	}
	++fireTimer;
}


void Turret::handleEvent(Event* pEvent) 
{
	if (pEvent->mType == EventType::TURRET_COMMAND)
	{
		// Try to get the necessary components to talk to.
		if (mpOwner->getParent() == nullptr)
			return;
		if (mpTransform == nullptr || mpParentTransform == nullptr || mpGLRect == nullptr || mpParentGLRect == nullptr)
		{
			mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
			mpParentTransform = static_cast<Transform*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_TRANSFORM));
			mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
			mpParentGLRect = static_cast<GLRect*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_GLRECT)); // We will want to mirror color.
			return;
		}

		mpGLRect->setColor(mpParentGLRect->getColor()); // Mirror color.

		TurretCommandEvent* pTurretEvent = static_cast<TurretCommandEvent*>(pEvent);
		this->mAimPoint = pTurretEvent->mAimPoint ; // Get aimpoint in local coordinates.

		float parentRotation = mpParentTransform->getRotation();

		glm::vec2 vec1 = mAimPoint - glm::vec2( mpParentTransform->getPosition() );
		vec1 = glm::normalize(vec1);
		
		glm::vec2 vec2 = glm::vec2(0.0f, 1.0f);

		//std::cout << "aim point is " << mAimPoint.x << ", " << mAimPoint.y << std::endl;

		if (vec1.x > 0)
			mAimAngle = -acosf(glm::dot(vec2, vec1));
		else
			mAimAngle = acosf(glm::dot( vec2, vec1));

		// Get the aim angle.
		//mAimAngle = glm::dot(vec1, vec2);
		mAimAngle = glm::degrees(mAimAngle) - parentRotation;
		//std::cout << "Aim angle is " << mAimAngle << std::endl;
		
		this->mpTransform->setRotation(mAimAngle);

		
		if (pTurretEvent->mShoot)
			mIsShooting = true;
		else
			mIsShooting = false;
	}
	/*
	else if (pEvent->mType == EventType::TRANSFORM_UPDATED)
	{
		TransformUpdatedEvent* pTransformEvent = static_cast<TransformUpdatedEvent*>(pEvent);
	}
	*/
}


void Turret::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{}

