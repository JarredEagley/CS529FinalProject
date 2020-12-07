#include "PhysicsManager.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/PhysicsBody.h"
#include "../GameObject.h"

PhysicsManager* PhysicsManager::instance = nullptr;
std::list<PhysicsBody*> PhysicsManager::gravityBodies; // Initialize the vector.
bool PhysicsManager::isPhysicsPaused = true;
float PhysicsManager::gameTimeMultiplier = 50.0f; // Game seconds per real life second.

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


float PhysicsManager::getGameTime()
{
	return GlobalManager::getFrameRateController()->getFrameTimeSec() * gameTimeMultiplier;
}


void PhysicsManager::Update()
{
	if (isPhysicsPaused)
		return;

	//float frameTime = GlobalManager::getFrameRateController()->getFrameTimeSec();
	float gameTime = getGameTime();

	// Integrate the physics bodies.
	for (auto pGOPair : GlobalManager::getGameObjectManager()->mGameObjects)
	{
		PhysicsBody* pPhysicsBody = static_cast<PhysicsBody*>(pGOPair.second->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		if (nullptr != pPhysicsBody)
			pPhysicsBody->Integrate(gameTime);
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
		// Create collide events.
		CollideEvent cEvent1;
		CollideEvent cEvent2;

		// Get the two bodies.
		PhysicsBody* pBody1 = pContact->mBodies[0];
		PhysicsBody* pBody2 = pContact->mBodies[1];

		// Are they approaching?
		glm::vec2 approachVector = (pBody2->mVelocity - pBody1->mVelocity) * (pBody2->mPosition - pBody1->mPosition);
		if ((approachVector.x + approachVector.y) < 0)
		{
			cEvent1.mObjectsAreApproaching = true;
			cEvent2.mObjectsAreApproaching = true;
		}

		// Create the collision normal. 
		cEvent1.mCollisionNormal = glm::normalize(pBody2->mPosition - pBody1->mPosition);
		cEvent2.mCollisionNormal = -cEvent1.mCollisionNormal;

		// Get the total speed.
		glm::vec2 totalVelocity = pBody1->mVelocity + pBody2->mVelocity;
		cEvent1.mTotalSpeed = glm::length(totalVelocity);
		cEvent2.mTotalSpeed = cEvent1.mTotalSpeed;

		// Get the collision angle.
		// Not 100% on my math here...
		cEvent1.mCollideAngle = glm::dot(totalVelocity, cEvent1.mCollisionNormal);
		cEvent2.mCollideAngle = -cEvent1.mCollideAngle;


		pBody1->handleEvent(&cEvent1);
		pBody2->handleEvent(&cEvent2);

		// / / / / // / //
		// old stuff


		// Let the two physics bodies know about eachother.
		
		/*
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

		// --- //

		// Send the collide event to the bodies.
		pContact->mBodies[0]->mpOwner->handleEvent(&cEvent);
		pContact->mBodies[1]->mpOwner->handleEvent(&cEvent);
		*/
	}
}
