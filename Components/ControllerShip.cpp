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

	// Get mouse position in world space.

	// EXAMPLE TIMED EVENT.
	if (pIM->isMouseButtonPressed(SDL_BUTTON_RIGHT))
	{
		DestroyAllProjectilesEvent* pDestroyEvent = new DestroyAllProjectilesEvent();
		pDestroyEvent->mTimer = 2000.0f; // Wait 2 seconds.
		GlobalManager::getEventManager()->addTimedEvent(pDestroyEvent);
	}

	// EXAMPLE GLOBAL EVENT.
	if (pIM->IsKeyPressed(SDL_SCANCODE_SPACE))
	{
		ExampleSpinEvent* pExampleEvent = new ExampleSpinEvent();
		GlobalManager::getEventManager()->broadcastEvent(pExampleEvent);
	}


	// Fire a turret command event.
	TurretCommandEvent* pNewTurretEvent = new TurretCommandEvent();
	if ( pIM->isMouseButtonPressed(SDL_BUTTON_LEFT) )
		pNewTurretEvent->mShoot = true;
	else
		pNewTurretEvent->mShoot = false;
	pNewTurretEvent->mAimPoint = mPlayerAimPoint;
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewTurretEvent);
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

