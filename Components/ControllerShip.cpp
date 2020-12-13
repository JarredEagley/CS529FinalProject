/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ControllerShip.cpp
	Purpose				Implementations for ControllerShip.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/7/2020

- End Header --------------------------------------------------------*/

#include "ControllerShip.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

ControllerShip::ControllerShip() : Component(ComponentTypes::TYPE_CONTROLLERSHIP), 
mAngularDamping(4.0f), mpShipData(nullptr)
{
}

ControllerShip::~ControllerShip()
{

}

void ControllerShip::Initialize() 
{
	GlobalManager::getEventManager()->Subscribe(EventType::CURSOR_WORLD_COORDS, mpOwner);
}

void ControllerShip::Update()
{
	// Necessary components to talk to.
	mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
	if ( mpShipData == nullptr)
	{
		std::cout << "Warning: Ship controller component failed to find its ship data component." << std::endl;
		return;
	}

	InputManager* pIM = GlobalManager::getInputManager();
	FrameRateController* pFRC = GlobalManager::getFrameRateController();

	// Rotation
	if (pIM->IsKeyPressed(SDL_SCANCODE_E))
	{
		mpShipData->applySpin(-1.0f);
	}
	else if (pIM->IsKeyPressed(SDL_SCANCODE_Q))
	{
		mpShipData->applySpin();
	}
	else
	{
		// Try to stop spinning.
		mpShipData->applyAngularDamping();
	}

	// Translation
	glm::vec2 translationForce = glm::vec2(0.0f);
	if (pIM->IsKeyPressed(SDL_SCANCODE_W))
	{
		translationForce.y += 1;
		//mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_S))
	{
		translationForce.y -= 1;
		//mpPhysicsBody->applyForce(-mpPhysicsBody->mForwardDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_A))
	{
		translationForce.x -= 1;
		//mpPhysicsBody->applyForce(-mpPhysicsBody->mRightDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_D))
	{
		translationForce.x += 1;
		//mpPhysicsBody->applyForce(mpPhysicsBody->mRightDir * mpShipData->mSecondaryAcceleration);
	}
	if (translationForce != glm::vec2(0.0f))
		translationForce = glm::normalize(translationForce);
	mpShipData->applyThrustSecondaryLocal(translationForce);

	// Main drive
	if (pIM->IsKeyPressed(SDL_SCANCODE_LSHIFT))
	{
		// Spool up
		mpShipData->throttleUp();
	}
	else if (pIM->IsKeyPressed(SDL_SCANCODE_LCTRL))
	{
		// Spool down
		mpShipData->throttleDown();
	}

	// Get mouse position in world space.

	// EXAMPLE TIMED EVENT.
	/*
	if (pIM->isMouseButtonPressed(SDL_BUTTON_RIGHT))
	{
		DestroyAllProjectilesEvent* pDestroyEvent = new DestroyAllProjectilesEvent();
		pDestroyEvent->mTimer = 1000.0f; // Wait 1 second.
		GlobalManager::getEventManager()->addTimedEvent(pDestroyEvent);
	}
	*/

	// Fire a turret command event.
	TurretCommandEvent* pNewTurretEvent = new TurretCommandEvent(mpOwner->mName);
	if ( pIM->isMouseButtonPressed(SDL_BUTTON_LEFT) )
		pNewTurretEvent->mShoot = true;
	else
		pNewTurretEvent->mShoot = false;
	pNewTurretEvent->mAimPoint = mPlayerAimPoint;
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewTurretEvent);

	// Fire missiles command event.
	if (pIM->IsKeyTriggered(SDL_SCANCODE_SPACE))
	{
		MissileLauncherCommandEvent* pNewMissileLauncherEvent = new MissileLauncherCommandEvent(mpOwner->mName);
		pNewMissileLauncherEvent->mFire = true;
		GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewMissileLauncherEvent);
	}
}


void ControllerShip::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::CURSOR_WORLD_COORDS)
	{
		CursorToWorldCoordinatesEvent* pCursorEvent = static_cast<CursorToWorldCoordinatesEvent*>(pEvent);
		this->mPlayerAimPoint = pCursorEvent->mCoords;
	}
}


void ControllerShip::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}

