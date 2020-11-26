#include "ControllerShip.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

ControllerShip::ControllerShip() : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
, mThrottle(0.0f), mThrottleSensitivity(0.5f)//, mMainAcceleration(1.0f), mSecondaryAcceleration(0.01f), mAngularAcceleration(0.1)
{
}

ControllerShip::~ControllerShip()
{

}

void ControllerShip::Update()
{
	// Necessary components to talk to.
	mpPhysicsBody = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
	if (mpPhysicsBody == nullptr || mpShipData == nullptr)
	{
		std::cout << "Warning: Ship controller component failed to find its physics and/or ship data component." << std::endl;
		return;
	}

	InputManager* pIM = GlobalManager::getInputManager();
	FrameRateController* pFRC = GlobalManager::getFrameRateController();

	// Rotation
	if (pIM->IsKeyPressed(SDL_SCANCODE_E))
	{
		mpPhysicsBody->applyTorque(-mpShipData->mAngularAcceleration);
	}
	else if (pIM->IsKeyPressed(SDL_SCANCODE_Q))
	{
		mpPhysicsBody->applyTorque(mpShipData->mAngularAcceleration);
	}
	else
	{
		// Try to stop spinning.
		mpPhysicsBody->applyTorque(-mpShipData->mAngularAcceleration * mpPhysicsBody->mAngularVelocity * 4.0);
	}

	// Translation
	if (pIM->IsKeyPressed(SDL_SCANCODE_W))
	{
		mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_S))
	{
		mpPhysicsBody->applyForce(-mpPhysicsBody->mForwardDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_A))
	{
		mpPhysicsBody->applyForce(-mpPhysicsBody->mRightDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_D))
	{
		mpPhysicsBody->applyForce(mpPhysicsBody->mRightDir * mpShipData->mSecondaryAcceleration);
	}

	// Main drive
	if (pIM->IsKeyPressed(SDL_SCANCODE_LSHIFT))
	{
		// Spool up
		this->mThrottle = std::min( mThrottle + mThrottleSensitivity , 100.0f );
	}
	else
	{
		// Spool down
		this->mThrottle = std::max(mThrottle - mThrottleSensitivity, 0.0f);
	}
	// Apply thrust.
	std::cout << "DEBUG - Current throttle is " << mThrottle << ", fuel is at " << mpShipData->mFuel << std::endl;
	if (mpShipData->mFuel > 0.0f && mThrottle > 1.0f) // Throttle actually has a tiny deadzone. Making this configurable could make for an interesting game mechanic.
	{
		mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mpShipData->mMainAcceleration * this->mThrottle);
	}
}

void ControllerShip::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
