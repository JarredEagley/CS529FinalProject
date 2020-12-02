#include "PhysicsManager.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/PhysicsBody.h"
#include "../GameObject.h"

PhysicsManager* PhysicsManager::instance = nullptr;
std::list<PhysicsBody*> PhysicsManager::gravityBodies; // Initialize the vector.

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


void PhysicsManager::Update()
{
	float frameTime = GlobalManager::getFrameRateController()->getFrameTime();
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
		PhysicsBody* pPhysicsBody1 = static_cast<PhysicsBody*>( pObjPair1->second->GetComponent(ComponentTypes::TYPE_PHYSICSBODY) );
		if (nullptr == pPhysicsBody1) continue;


		auto pObjPair2 = pObjPair1; // TO-DO: If this is not creating a copy, switch to std::map
		pObjPair2++;
		for (;
			pObjPair2 != GlobalManager::getGameObjectManager()->mGameObjects.end();
			++pObjPair2)
		{
			PhysicsBody* pPhysicsBody2 = static_cast<PhysicsBody*>(pObjPair2->second->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
			if (nullptr == pPhysicsBody2) continue;

			// Check ignore collision.
			if (pPhysicsBody1->getIgnoredPhysicsBody() == pPhysicsBody2
				|| pPhysicsBody2->getIgnoredPhysicsBody() == pPhysicsBody1)
				continue;

			// If we're here, we have two GO's with body components.
			GlobalManager::getCollisionManager()->checkCollisionAndGenerateContact(pPhysicsBody1->mpShape, pPhysicsBody1->mPosition, pPhysicsBody2->mpShape, pPhysicsBody2->mPosition);
		}
	}

	// Resolve contacts...

	for (auto pContact : GlobalManager::getCollisionManager()->mContacts)
	{
		// Create a collide event.
		CollideEvent cEvent;

		// Let the two physics bodies know about eachother.
		cEvent.mpBodies[0] = pContact->mBodies[0];
		cEvent.mpBodies[1] = pContact->mBodies[1];

		// --- // 
		glm::vec2 vel0 = cEvent.mpBodies[0]->mVelocity;
		glm::vec2 vel1 = cEvent.mpBodies[1]->mVelocity;
		glm::vec2 pos0 = cEvent.mpBodies[0]->mPosition;
		glm::vec2 pos1 = cEvent.mpBodies[1]->mPosition;
		float mass0 = cEvent.mpBodies[0]->mMass;
		float mass1 = cEvent.mpBodies[1]->mMass;

		// Check if the two objects are approaching.
		glm::vec2 approachVec = (vel1 - vel0) * (pos1 - pos0);
		if ((approachVec.x + approachVec.y) >= 0)
			continue; 

		glm::vec2 collisionNormal = glm::normalize( pos0 - pos1 ); 

		//std::cout << "DEBUG - normal is " << collisionNormal.x << ", " << collisionNormal.y << std::endl;

		float totalMassInv = 1 / (cEvent.mpBodies[0]->mMass + cEvent.mpBodies[1]->mMass);
		float massDiff01 = cEvent.mpBodies[0]->mMass - cEvent.mpBodies[1]->mMass;
		float massDiff10 = cEvent.mpBodies[1]->mMass - cEvent.mpBodies[0]->mMass;

		// Equations according to wikipedia for elastic collision.
		float v0Term2 = (2.0f  * mass1) / ( mass0 + mass1);
		float v1Term2 = (2.0f  * mass0) / ( mass0 + mass1);

		glm::vec2 temp;
		float templensqr;

		temp = pos0 - pos1;
		templensqr = (temp.x * temp.x) + (temp.y * temp.y);
		float v0Term3 = glm::dot( (vel0-vel1) , (pos0-pos1) ) / templensqr;
		temp =  pos1 - pos0;
		templensqr = (temp.x * temp.x) + (temp.y * temp.y);
		float v1Term3 = glm::dot( (vel1-vel0) , (pos1-pos0) ) / templensqr;

		glm::vec2 v0Term4 = pos0 - pos1;
		glm::vec2 v1Term4 = pos1 - pos0;


		// Combine velocities.
		cEvent.mNewVel0 = vel0 - (v0Term2 * v0Term3 * v0Term4);
		cEvent.mNewVel1 = vel1 - (v1Term2 * v1Term3 * v1Term4);

		//std::cout << "DEBUG - new vel 0 is " << cEvent.mNewVel0.x << ", "<< cEvent.mNewVel0.y << std::endl;
		//std::cout << "DEBUG - new vel 1 is " << cEvent.mNewVel1.x << ", "<< cEvent.mNewVel1.y << std::endl;

		// TO-DO: Perhaps make collision inelastic?

		// --- //

		// Send the collide event to the bodies.
		pContact->mBodies[0]->mpOwner->handleEvent(&cEvent);
		pContact->mBodies[1]->mpOwner->handleEvent(&cEvent);
	}
}
