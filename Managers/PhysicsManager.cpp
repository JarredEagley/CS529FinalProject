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

		// Inform each about the other.
		cEvent2.mpOtherBody = pBody1;
		cEvent1.mpOtherBody = pBody2;

		collisionType collisionType1 = pBody1->mCollisionType;
		collisionType collisionType2 = pBody2->mCollisionType;

		// Are they approaching?
		glm::vec2 approachVector = (pBody2->mVelocity - pBody1->mVelocity) * (pBody2->mPosition - pBody1->mPosition);
		if ((approachVector.x + approachVector.y) < 0)
		{
			cEvent1.mObjectsAreApproaching = true;
			cEvent2.mObjectsAreApproaching = true;
		}

		// If either is noclip...
		if (
			collisionType1 == collisionType::NOCLIP
			|| collisionType2 == collisionType::NOCLIP
			|| !cEvent1.mObjectsAreApproaching
			|| !cEvent2.mObjectsAreApproaching
			)
		{
			// Just pass through eachother.
			cEvent1.mResponse = CollideEvent::collisionResponse::PASS;
			cEvent2.mResponse = CollideEvent::collisionResponse::PASS;
		}
		else
		{
			glm::vec2 relativeVelocity = pBody1->mVelocity - pBody2->mVelocity;
			cEvent1.mRelativeVelocity = relativeVelocity;
			cEvent2.mRelativeVelocity = relativeVelocity;
			float relativeSpeedSqr = relativeVelocity.x * relativeVelocity.x + relativeVelocity.y * relativeVelocity.y;
			
			float const PIERCING_THRESHOLD = 10.0f; // Need to move this to the manager.

			// Create the collision normal. 
			glm::vec2 collisionNormal = glm::normalize(pBody2->mPosition - pBody1->mPosition);

			// Above certain speed squared.
			if (relativeSpeedSqr > PIERCING_THRESHOLD)
			{
				// Pierce (go through, do more damage!)
				cEvent1.mResponse = CollideEvent::collisionResponse::PIERCE;
				cEvent2.mResponse = CollideEvent::collisionResponse::PIERCE;

				cEvent1.mResistance = pBody1->mpShape->mArea ;
				cEvent2.mResistance = pBody2->mpShape->mArea ;
			}
			else
			{
				// Deflect
				cEvent1.mResponse = CollideEvent::collisionResponse::DEFLECT;
				cEvent2.mResponse = CollideEvent::collisionResponse::DEFLECT;

				float combinedMassInverse = 1.0f/(pBody1->mMass + pBody2->mMass);
				glm::vec2 relativePosition = pBody1->mPosition - pBody2->mPosition;
				float distSqr = relativePosition.x*relativePosition.x + relativePosition.y * relativePosition.y;

				float velDotPos1 = glm::dot(pBody1->mVelocity - pBody2->mVelocity, pBody1->mPosition - pBody2->mPosition);
				float velDotPos2 = glm::dot(pBody2->mVelocity - pBody1->mVelocity ,pBody2->mPosition - pBody1->mPosition);

				// Calculate new velocity for body 1.
				cEvent1.mNewVelocity =
					pBody1->mVelocity - (2.0f * pBody2->mMass * combinedMassInverse) 
					* (velDotPos1/distSqr) * (pBody1->mPosition - pBody2->mPosition);

				// Calculate new velocity for body 2.
				cEvent2.mNewVelocity =
					pBody2->mVelocity - (2.0f * pBody1->mMass * combinedMassInverse)
					* (( velDotPos2) / (distSqr)) * ( pBody2->mPosition - pBody1->mPosition);
			}

		}

		// Broadcast events.
		pBody1->handleEvent(&cEvent1);
		pBody2->handleEvent(&cEvent2);

	}
}
