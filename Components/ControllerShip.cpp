#include "ControllerShip.h"
#include "ComponentTypes.h"
#include "PhysicsBody.h"
#include "../Managers/GlobalManager.h"

ControllerShip::ControllerShip() : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
{

}

ControllerShip::ControllerShip(float speed) : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
{

}

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

	if (pIM->IsKeyPressed(SDL_SCANCODE_E))
	{
		pPhys->mTotalTorque = 1.0f;
	}
	if (pIM->IsKeyPressed(SDL_SCANCODE_Q))
	{
		pPhys->mTotalTorque = -1.0f;
	}

}

void ControllerShip::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
