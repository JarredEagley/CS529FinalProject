/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Explosion.cpp
	Purpose:			Implementations for Explosion.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/8/2020

- End Header --------------------------------------------------------*/

#include "Explosion.h"
#include "../Managers/GlobalManager.h"

Explosion::Explosion() : Component(ComponentTypes::TYPE_EXPLOSION),
mpPhysicsBody(nullptr), mpTransform(nullptr), mpRect(nullptr)
{
}

Explosion::~Explosion()
{
}


void Explosion::Initialize()
{
}

void Explosion::Update()
{
	// Explosion always starts at 0.1 size.
	// Input is the intensity.
	// Rate of change is same for size and intensity.
	// For now, fixed rate of change. If I need to change this later, I will.
	if (mpPhysicsBody == nullptr || mpTransform == nullptr || mpRect == nullptr)
	{
		mpPhysicsBody = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		mpRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		return;
	}

	if (mIntensity > 0)
	{
		//std::cout << "Explosion update: Intensity is " << mIntensity << ", and size is " << mSize << std::endl;

		float deltaTime = GlobalManager::getPhysicsManager()->getGameTime();
		// Decrement intensity.
		mIntensity -= deltaTime * GlobalManager::getPhysicsManager()->explosionExpansionRate;
		// Increment size.
		mSize += deltaTime * GlobalManager::getPhysicsManager()->explosionExpansionRate;
	}
	else
	{
		mpOwner->mIsMarkedForDelete = true;
		return;
	}

	mpTransform->setScale(glm::vec2(mSize));
	mpRect->setAlpha( mIntensity/mStartIntensity );
	if (mpPhysicsBody->mpShape->mType == Shape::ShapeType::CIRCLE)
	{
		ShapeCircle* pShape = static_cast<ShapeCircle*>(mpPhysicsBody->mpShape);
		pShape->mRadius = mSize*0.5f;
		// Note: Not going to set mass in here too; an explosion's radius increases and its density decreases. Not that that'll be needed for anything.
	}
	else
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Error::Explosion: Shape type for '" << mpOwner->mName << "' was not circle. Failed to update physics shape." << std::endl;
	}

}


void Explosion::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::COLLIDE)
	{
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);

		PhysicsBody* pOtherBody = pCollideEvent->mpOtherBody;
		float distance = pCollideEvent->mDistance;
		if (distance == 0.0f)
			distance = 0.01f;

		// Deal damage based on distance, size and intensity.
		// Dividing multiplier by 1000 gives slightly more comfortable config control.
		float damage = ((mSize*0.5f) / distance) * mIntensity * (GlobalManager::getPhysicsManager()->explosionDamageMultiplier/1000.0f);

		DoDamageEvent* pDamageEvent = new DoDamageEvent(damage);
		pOtherBody->mpOwner->handleEvent(pDamageEvent);
	}
}


void Explosion::setUniformData(ShaderProgram* pProgram)
{}


void Explosion::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) 
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Intensity") && inputObj["Intensity"].IsNumber())
	{
		this->mIntensity = inputObj["Intensity"].GetFloat();
		this->mStartIntensity = mIntensity;
	}
	else
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
		{
			std::cout << "Error: Explosion component failed to deserialize; Intensity was missing or incorrectly formatted. This may create an orphan game object!" << std::endl;
		}
	}
}
