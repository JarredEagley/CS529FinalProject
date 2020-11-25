#include "ControllerShip.h"
#include "ComponentTypes.h"
#include "PhysicsBody.h"
#include "../Managers/GlobalManager.h"

ControllerShip::ControllerShip() : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
, mThrottle(0.0f), mMainAcceleration(1.0f), mSecondaryAcceleration(0.01f), mAngularAcceleration(0.1)
{
}

/*
ControllerShip::ControllerShip(float speed) : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
{

}
*/

ControllerShip::~ControllerShip()
{

}

void ControllerShip::Update()
{
	PhysicsBody* pPhys = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	if (pPhys == nullptr)
		return;

	InputManager* pIM = GlobalManager::getInputManager();
	FrameRateController* pFRC = GlobalManager::getFrameRateController();

	// Rotation
	if (pIM->IsKeyPressed(SDL_SCANCODE_E))
	{
		pPhys->applyTorque(-mAngularAcceleration);
	}
	else if (pIM->IsKeyPressed(SDL_SCANCODE_Q))
	{
		pPhys->applyTorque(mAngularAcceleration);
	}
	else
	{
		// Try to stop.
		pPhys->applyTorque(-mAngularAcceleration * pPhys->mAngularVelocity * 2.0);
	}

	// Translation
	if (pIM->IsKeyPressed(SDL_SCANCODE_W))
	{
		pPhys->applyForce(pPhys->mForwardDir * mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_S))
	{
		pPhys->applyForce(-pPhys->mForwardDir * mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_A))
	{
		pPhys->applyForce(-pPhys->mRightDir * mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_D))
	{
		pPhys->applyForce(pPhys->mRightDir * mSecondaryAcceleration);
	}


	// Main drive

}

void ControllerShip::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
