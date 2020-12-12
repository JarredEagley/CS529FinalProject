#include "PhysicsManager.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/PhysicsBody.h"
#include "../GameObject.h"


PhysicsManager* PhysicsManager::instance = nullptr;
std::list<PhysicsBody*> PhysicsManager::gravityBodies; // Initialize the vector.
//bool PhysicsManager::isPhysicsPaused = true;

// All of these get replaced by the config reading-- they're only here just in case.
float PhysicsManager::gameTimeMultiplier = 50.0f; // Game seconds per real life second.
float PhysicsManager::piercingThreshold = 40.0f;
float PhysicsManager::piercingDamageMultiplier = 3.0f;
float PhysicsManager::deflectDamageMultiplier = 1.0f;
float PhysicsManager::explosionDamageMultiplier = 1.0f;

float PhysicsManager::explosionExpansionRate = 1.0f;

float PhysicsManager::universalGravitationalConstant = 0.0000000000667f;

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


glm::vec2 PhysicsManager::calculateGravitationalForces(glm::vec2 position)
{
	float const G_CONST = universalGravitationalConstant;

	glm::vec2 result = glm::vec2(0.0f);

	for (auto pBody : gravityBodies)
	{
		// Technically correct, but I'm removing the this-> mass term. Its negligable anyways.
		//float numerator = (G_CONST * this->mMass * pBody->mMass);
		float numerator = (G_CONST * pBody->mMass);

		float denominator = (pBody->mPosition.x - position.x) * (pBody->mPosition.x - position.x)
			+ (pBody->mPosition.y - position.y) * (pBody->mPosition.y - position.y);

		// Guard against same position.
		if (denominator == 0.0f)
			continue;

		// Scale the force.
		float gravScale = numerator / (denominator);
		gravScale /= 1000.0 * 1000.0; // Convert m^2 to km^2

		//std::cout << "DEBUG - Grav scale is " << gravScale << "\n";

		// I tried to avoid needing to normalize. I might try again if I have time.

		glm::vec2 gravitationalForce = glm::normalize(pBody->mPosition - position) * gravScale / 1000.0f;

		//std::cout << "DEBUG - grav force: (" << gravitationalForce.x << ", " << gravitationalForce.y << ")" << std::endl;
		// We do not apply gravitational forces that don't make sense here.
		if (
			isnan(gravitationalForce.x)
			|| isnan(gravitationalForce.y)
			|| isinf(gravitationalForce.x)
			|| isinf(gravitationalForce.y)
			)
			return glm::vec2(0.0f);

		//applyForce(gravitationalForce);
		result += gravitationalForce;
	}

	return result;
}


void PhysicsManager::Update()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
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

		if (pBody1 == nullptr || pBody2 == nullptr)
			continue;

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

		// Get their relative speed and velocity.
		glm::vec2 relativeVelocity = pBody1->mVelocity - pBody2->mVelocity;
		cEvent1.mNGB_RelativeVelocity = relativeVelocity;
		cEvent2.mNGB_RelativeVelocity = relativeVelocity;

		float relativeSpeedSqr = relativeVelocity.x * relativeVelocity.x + relativeVelocity.y * relativeVelocity.y;
		float relativeSpeed = sqrt(relativeSpeedSqr);
		cEvent1.mNGB_RelativeSpeed = relativeSpeed;
		cEvent2.mNGB_RelativeSpeed = relativeSpeed;

		// Get relative position, since its used in squared distance.
		glm::vec2 relativePosition = pBody1->mPosition - pBody2->mPosition;

		// Get distance squared, since its used in two places.
		float distSqr = relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y;

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

			// Store distance, incase one is an explosion.
			float distance = sqrt(distSqr);
			cEvent1.mDistance = distance;
			cEvent2.mDistance = distance;

			//std::cout << "TEST: " << pBody1->mpOwner->mName <<", " << pBody2->mpOwner->mName << "\n";
		}
		// The two types of collision:
		else
		{
			// Create the collision normal. 
			glm::vec2 collisionNormal = glm::normalize(pBody2->mPosition - pBody1->mPosition);

			// Above certain speed squared.
			if (relativeSpeedSqr > GlobalManager::getPhysicsManager()->piercingThreshold)
			{
				// Pierce (go through, do more damage!)
				cEvent1.mResponse = CollideEvent::collisionResponse::PIERCE;
				cEvent2.mResponse = CollideEvent::collisionResponse::PIERCE;
			}
			else
			{
				// Deflect
				cEvent1.mResponse = CollideEvent::collisionResponse::DEFLECT;
				cEvent2.mResponse = CollideEvent::collisionResponse::DEFLECT;

				float combinedMassInverse = 1.0f/(pBody1->mMass + pBody2->mMass);

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

		// Broadcast events to the gameobjects.
		pBody1->mpOwner->handleEvent(&cEvent1);
		pBody2->mpOwner->handleEvent(&cEvent2);
	}
}



