#include "PhysicsManager.h"
#include "GlobalManager.h"
#include "../Components/PhysicsDynamic.h"
#include "../GameObject.h"

PhysicsManager* PhysicsManager::instance = nullptr;

void PhysicsManager::destroySingleton()
{
	delete instance;
}

// End of singleton stuff //
// ------------------------------------------------------ //

CollideEvent::CollideEvent() : Event(EventType::COLLIDE)
{}

CollideEvent::~CollideEvent()
{}

// End of CollideEvent stuff //
// ------------------------------------------------------ //

// Physics manager's private constructor.
PhysicsManager::PhysicsManager()
{
}

void PhysicsManager::Update(float frameTime)
{
	for (auto pGOPair : GlobalManager::getGameObjectManager()->mGameObjects)
	{
		PhysicsDynamic* PhysicsDynamic = static_cast<PhysicsDynamic*>();
		if (nullptr != PhysicsDynamic)
			pBody->Integrate(frameTime);
	}
}
