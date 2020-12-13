/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			MissileLauncher.cpp
	Purpose				Implementations for the missile launcher component.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#include "MissileLauncher.h"
#include "../Managers/GlobalManager.h"
#include <time.h>
#include "AIMissile.h"
#include "Transform.h"

MissileLauncher::MissileLauncher() : Component(ComponentTypes::TYPE_MISSILELAUNCHER)
{
}

MissileLauncher::~MissileLauncher()
{
}


void MissileLauncher::Initialize()
{
	// Subscribe to target lock events. Will only be responded to if we're parented to the player.
	GlobalManager::getEventManager()->Subscribe(EventType::TARGET_LOCK, this->mpOwner);
}

void MissileLauncher::Update()
{

	if (mpGLRect == nullptr || mpTransform == nullptr)
	{
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	GameObject* pParentGO = mpOwner->getParent();
	if (pParentGO == nullptr)
		return;

	GLRect* pParentRect = static_cast<GLRect*>(pParentGO->GetComponent(ComponentTypes::TYPE_GLRECT));
	if (pParentRect == nullptr)
		return;

	// Mirror parent color, like turrets.
	mpGLRect->setColor(pParentRect->getColor());

	// Handle pausing down here so we still get color set correctly.
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;

	// -- launching -- //

	// We need parent's ship data and physics.
	ShipData* pParentShipData = static_cast<ShipData*>(pParentGO->GetComponent(ComponentTypes::TYPE_SHIPDATA));
	Transform* pParentTransform = static_cast<Transform*>(pParentGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	PhysicsBody* pParentPhysics = static_cast<PhysicsBody*>(pParentGO->GetComponent(ComponentTypes::TYPE_PHYSICSBODY)); 

	// Launch hotkey is spacebar.
	if (GlobalManager::getInputManager()->IsKeyTriggered(SDL_SCANCODE_SPACE))
	{
		if (mTargetGOName == "")
			return;

		GameObject* pTargetGO = GlobalManager::getGameObjectManager()->getGameObject(mTargetGOName);
		if (pTargetGO == nullptr)
			return;

		// We have a valid target. Fire the missile!
		GameObjectFactory* pGOF = GlobalManager::getGameObjectFactory();
		time_t currentTime = time(NULL);
		std::string missileName = "Missile_" + std::to_string(currentTime);
		std::string missilePath = GlobalManager::getResourceManager()->pathProjectiles + "GuidedMissile.json";
		std::string indicatorName = missileName + "_Indicator";
		std::string indicatorPath = GlobalManager::getResourceManager()->pathIndicators + "NuclearMissileIndicator.json";

		GameObject* pNewMissile = pGOF->createDynamicGameObject(missilePath, missileName);
		GameObject* pNewMissileIndicator = pGOF->createDynamicGameObject(indicatorPath, indicatorName);
		pNewMissileIndicator->setParent(pNewMissile->mName);

		// Apply transforms, forces, target...

		if (pNewMissile == nullptr)
			return;

		Transform* pMissileTransform = static_cast<Transform*>(pNewMissile->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		PhysicsBody* pMissilePhys = static_cast<PhysicsBody*>(pNewMissile->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		AIMissile* pMissileAI = static_cast<AIMissile*>(pNewMissile->GetComponent(ComponentTypes::TYPE_AI_MISSILE));
	
		if (pMissileTransform == nullptr || pMissileAI == nullptr || pMissilePhys == nullptr)
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Error::MissileLauncher: One or more missile gameobject's components were NULL. Deleting missile." << std::endl;
			pNewMissile->mIsMarkedForDelete = true;
			return;
		}
		
		// Transform.
		pMissileTransform->setPosition(pParentTransform->getPosition());

		//std::cout << "Missile at: " << pMissileTransform->getPosition().x << ", " << pMissileTransform->getPosition().y << std::endl;

		// Velocity.
		pMissilePhys->mVelocity = pParentPhysics->mVelocity;

		// Forces.
		pMissilePhys->mTotalForce = glm::vec2(0.0f);
		//pMissilePhys->applyForce(mLaunchForce); // TO-DO: Launch force relative direction?

		// Target
		pMissileAI->mTargetName = mTargetGOName;
	}
}

void MissileLauncher::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::TARGET_LOCK)
	{
		// Only respond if we're parented to the player.
		GameObject* pParent = mpOwner->getParent();
		if (pParent == nullptr || pParent->mName != "PLAYER")
			return;

		TargetLockEvent* pTargetEvent = static_cast<TargetLockEvent*>(pEvent);
		this->mTargetGOName = pTargetEvent->mTargetGOName;
	}
}


void MissileLauncher::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Launch Force") && inputObj["Launch Force"].IsArray() && inputObj["Launch Force"].GetArray().Size() >= 2)
	{
		auto arr = inputObj["Launch Force"].GetArray();

		if (arr[0].IsNumber() && arr[1].IsNumber())
		{
			this->mLaunchForce = glm::vec2(
				arr[0].GetFloat(),
				arr[1].GetFloat()
			);
		}
		else
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Missile Launcher launch force was improperly formatted. Default used." << std::endl;
		}
	}


}