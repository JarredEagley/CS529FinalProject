#include "ControllerShip.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

ControllerShip::ControllerShip() : Component(ComponentTypes::TYPE_CONTROLLERSHIP), 
mAngularDamping(4.0f)
{
}

ControllerShip::~ControllerShip()
{

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
		translationForce.x += 1;
		//mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_S))
	{
		translationForce.x -= 1;
		//mpPhysicsBody->applyForce(-mpPhysicsBody->mForwardDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_A))
	{
		translationForce.y += 1;
		//mpPhysicsBody->applyForce(-mpPhysicsBody->mRightDir * mpShipData->mSecondaryAcceleration);
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_D))
	{
		translationForce.y -= 1;
		//mpPhysicsBody->applyForce(mpPhysicsBody->mRightDir * mpShipData->mSecondaryAcceleration);
	}
	if (translationForce != glm::vec2(0.0f))
		translationForce = glm::normalize(translationForce);
	mpShipData->applyThrustSecondary(translationForce);

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
	// Apply thrust.
	//std::cout << "DEBUG - Current throttle is " << mThrottle << ", fuel is at " << mpShipData->mFuel << std::endl;
	/*
	if (mpShipData->mFuel > 0.0f && mpShipData->mThrottle > 1.0f) // Throttle actually has a tiny deadzone. Making this configurable could make for an interesting game mechanic.
	{
		mpShipData->mFuel -=  GlobalManager::getFrameRateController()->getFrameTimeSec() / mpShipData->mFuelEfficiency;
		mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mpShipData->mMainAcceleration * (mpShipData->mThrottle / 100.0f));
	}
	*/
}

void ControllerShip::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
