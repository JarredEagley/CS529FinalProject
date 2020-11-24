#include "PhysicsManager.h"
#include "GlobalManager.h"
#include "../Components/PhysicsBody.h"
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
		PhysicsBody* PhysicsBody = static_cast<PhysicsBody*>();
		if (nullptr != PhysicsBody)
			pBody->Integrate(frameTime);
	}
}
