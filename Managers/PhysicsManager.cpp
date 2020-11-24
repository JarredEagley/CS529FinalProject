#include "PhysicsManager.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/PhysicsBody.h"
#include "../GameObject.h"

PhysicsManager* PhysicsManager::instance = nullptr;
auto PhysicsManager::gravityBodies; // Initialize the vector.


void PhysicsManager::destroySingleton()
{
	// The bodies pointed to in the vector belong to their game objects; game objects will dispose of them.
	gravityBodies.clear();
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
	// Integrate the physics bodies.
	for (auto pGOPair : GlobalManager::getGameObjectManager()->mGameObjects)
	{
		PhysicsBody* pPhysicsBody = static_cast<PhysicsBody*>(pGOPair.second->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		if (nullptr != pPhysicsBody)
			pPhysicsBody->Integrate(frameTime);
	}

	// Reset previous contacts.
	GlobalManager::getCollisionManager()->Reset();

	for (auto pObjPair1 = GlobalManager::getGameObjectManager()->mGameObjects.begin();
		pObjPair1 != GlobalManager::getGameObjectManager()->mGameObjects.end();
		++pObjPair1)
	{
		
		PhysicsBody* pPhysicsBody1 = static_cast<PhysicsBody*>( (*pObjPair1)->second->GetComponent(ComponentTypes::TYPE_PHYSICSBODY) );
		if (nullptr == pPhysicsBody1) continue;

		for (auto pObjPair2 = pObjPair1 + 1;
			pObjPair2 != GlobalManager::getGameObjectManager()->mGameObjects.end();
			++pObjPair2)
		{
			PhysicsBody* pPhysicsBody2 = static_cast<PhysicsBody*>((*pObjPair2)->second->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
			if (nullptr == pPhysicsBody2) continue;

			// If we're here, we have two GO's with body components.
			GlobalManager::getCollisionManager()->checkCollisionAndGenerateContact(pPhysicsBody1->mpShape, pPhysicsBody1->mPosition, pPhysicsBody2->mpShape, pPhysicsBody2->mPosition);
		}
	}

	// TO-DO: Advanced physics resolutions go here.
}
