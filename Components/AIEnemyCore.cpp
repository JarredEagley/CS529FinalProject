
#include "AIEnemyCore.h"
#include "../Managers/GlobalManager.h"

#include "glm/gtx/projection.hpp"

AIEnemyCore::AIEnemyCore() : Component(ComponentTypes::TYPE_AI_ENEMYCORE) {}

AIEnemyCore::~AIEnemyCore()
{
	// Remove from the enemies list.
	GlobalManager::getGameStateManager()->mLivingEnemies.remove(this->mpOwner->mName);
}

void AIEnemyCore::Initialize()
{
	// Register with the enemies list.
	GlobalManager::getGameStateManager()->mLivingEnemies.push_back(this->mpOwner->mName);
}


void AIEnemyCore::Update()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;

	if (mpShipData == nullptr || mpPhysicsBody == nullptr)
	{
		mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
		mpPhysicsBody = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		return;
	}

	// Find the nearest gravitational body.
	getNearestGravityBody();

}

void AIEnemyCore::handleEvent(Event* pEvent)
{
}

// AI Functions

void AIEnemyCore::getNearestGravityBody()
{
	glm::vec2 myPosition = mpPhysicsBody->mPosition;

	// Get nearest
	mpNearestGravityBody = nullptr;
	mNearestGravityBodyDistanceSquared = -1.0f;
	for (auto pGBody : GlobalManager::getPhysicsManager()->gravityBodies)
	{
		glm::vec2 thisBodyPos = pGBody->mPosition;
		float thisDistSqr = (thisBodyPos.x - myPosition.x) * (thisBodyPos.x - myPosition.x)
			+ (thisBodyPos.y - myPosition.y) * (thisBodyPos.y - myPosition.y);

		if (mNearestGravityBodyDistanceSquared == -1.0f || mNearestGravityBodyDistanceSquared > thisDistSqr)
		{
			mpNearestGravityBody = pGBody;
			mNearestGravityBodyDistanceSquared = thisDistSqr;
		}
	}

	// If we did not find any...
	if (mpNearestGravityBody == nullptr)
	{
		mNearestGravityBodyDistanceSquared = 0.0f;
		mNearestGravityBodyDistance = 0.0f;
		isInGravity = false;
	}
	// If we found one...
	else
	{
		// Precompute distance.
		mNearestGravityBodyDistance = sqrt(mNearestGravityBodyDistanceSquared);
		isInGravity = true;
	}
}

void AIEnemyCore::calculateOrbitalParameters()
{

}

void AIEnemyCore::keepOrbit(float closestDistSqr, PhysicsBody* pClosest)
{
	// Gravity
	float const G_CONST = GlobalManager::getPhysicsManager()->universalGravitationalConstant;

	// Unfortunately, orbital mechanics involves quite a few sqrts sometimes.
	float orbitalRadius = sqrt(closestDistSqr);
	float orbitalSpeedAtCurrentAltitude = sqrt((G_CONST * pClosest->mMass) / (orbitalRadius * 1000.0f));
	orbitalSpeedAtCurrentAltitude /= 1000.0f;

	//std::cout << "orbital speed = " << orbitalSpeedAtCurrentAltitude << std::endl;
	//std::cout << "current speed = " << glm::length(mpPhysicsBody->mVelocity) << std::endl;

	glm::vec2 relativeVelocity = mpPhysicsBody->mVelocity - pClosest->mVelocity;
	glm::vec2 relativePosition = mpPhysicsBody->mPosition - pClosest->mPosition;
	glm::vec2 relativePositionNormal = glm::vec2(relativePosition.y, -relativePosition.x);

	// More unfortuantely expensive calculations.
	glm::vec2 progradeVelocity = glm::proj(relativeVelocity, relativePositionNormal);	// forward/backwards
	glm::vec2 radialVelocity = glm::proj(relativeVelocity, relativePosition);			// up/down


	//std::cout << "Prograde:" << progradeVelocity.x << ", " << progradeVelocity.y << std::endl;
	//std::cout << "Radial:" << radialVelocity.x << ", " << radialVelocity.y << std::endl;

	//std::cout << "Orbital radius is " << orbitalRadius << " and desired altitiude is " << mDesiredAltitude << std::endl;

	// Go up or down.
	if (orbitalRadius > mDesiredAltitude + mOrbitThickness)
	{
		// Try go down
		// Cut our orbital velocity down a little.
		float desiredSpeed = orbitalSpeedAtCurrentAltitude * (1.0f - mOrbitalAdjustmentAgression);
		// Aim our velocity vector down a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(relativePosition) * orbitalSpeedAtCurrentAltitude * (0.0f - mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(progradeVelocity) * orbitalSpeedAtCurrentAltitude * (1.0f - mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		//std::cout << "dvr: " << desiredVelocityRadial.x << ", " << desiredVelocityRadial.y << std::endl;

		matchVelocityVector(desiredVelocityVector + pClosest->mVelocity);
	}
	else if (orbitalRadius < mDesiredAltitude - mOrbitThickness)
	{
		// Try go up
		// Increase our velocity a little
		float desiredSpeed = orbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		// Aim our velocity vector up a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(relativePosition) * orbitalSpeedAtCurrentAltitude * (0.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(progradeVelocity) * orbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		matchVelocityVector(desiredVelocityVector + pClosest->mVelocity);
	}
	else
	{
		// Try to maintain proper orbital velocity.
		float desiredSpeed = orbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(progradeVelocity) * orbitalSpeedAtCurrentAltitude;

		matchVelocityVector(desiredVelocityPrograde + pClosest->mVelocity);
	}
}


void AIEnemyCore::matchVelocityVector(glm::vec2 desiredVelocity)
{
	glm::vec2 currentVelocity = mpPhysicsBody->mVelocity;
	float speedDiff = glm::distance(desiredVelocity, currentVelocity);
	glm::vec2 velocityDiff = desiredVelocity - currentVelocity;
	glm::vec2 velocityDiffNormalized = glm::normalize(velocityDiff);

	//std::cout << "Desired speed is " << glm::length(desiredVelocity) << std::endl;
	//std::cout << "Speed diff: " << speedDiff << std::endl;

	// Use drive if speed above a threshold and fuel not depleted.
	if (speedDiff > mManeuveringSpeedThreshold && mpShipData->mFuel > 0.1f)
	{
		// Align
		float alignmentAmount = alignToVector(-velocityDiffNormalized);

		//std::cout << "Throttle " << (1.0f / abs(alignmentAmount) - 1.0f) * 1.5f << std::endl;

		// Accelerate
		if (alignmentAmount == 0)
			mpShipData->setThrottle(100.0f);
		else
			mpShipData->setThrottle((1.0f / abs(alignmentAmount) - 1.0f) * 1.5f);
	}
	else
	{
		// Do orientation lock.
		preferredOrientation();
	}
	// Use maneuvering thrusters regardless.
	mpShipData->applyThrustSecondary(velocityDiffNormalized);

}

float AIEnemyCore::alignToVector(glm::vec2 alignmentVector)
{
	glm::vec2 myNormal = mpPhysicsBody->mRightDir;
	float alignmentAmount = glm::dot(alignmentVector, myNormal);

	// Turning +
	if (alignmentAmount > 0)
	{
		// Alignment amount = desired turn speed.
		if (mpPhysicsBody->mAngularVelocity > alignmentAmount)
			mpShipData->applySpin(-alignmentAmount);
		else
			mpShipData->applySpin(alignmentAmount);
	}
	// Turning -
	else
	{
		// Alignment amount = desired turn speed.
		if (mpPhysicsBody->mAngularVelocity < alignmentAmount)
			mpShipData->applySpin(-alignmentAmount);
		else
			mpShipData->applySpin(alignmentAmount);
	}

	return alignmentAmount;
}

void AIEnemyCore::preferredOrientation()
{
	if (mOrientationBehavior == "FACE_PLAYER")
	{

	}
	else if (mOrientationBehavior == "FACE_PROGRADE")
	{

	}
	// By default, no need to do anything else; just use the natural angular damping specified in ship data.
}






void AIEnemyCore::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{



}