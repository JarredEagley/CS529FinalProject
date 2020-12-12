
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
	mNGB_DistSqr = -1.0f; // NGB = nearest gravity body.
	for (auto pGBody : GlobalManager::getPhysicsManager()->gravityBodies)
	{
		glm::vec2 thisBodyPos = pGBody->mPosition;
		float thisDistSqr = (thisBodyPos.x - myPosition.x) * (thisBodyPos.x - myPosition.x)
			+ (thisBodyPos.y - myPosition.y) * (thisBodyPos.y - myPosition.y);

		if (mNGB_DistSqr == -1.0f || mNGB_DistSqr > thisDistSqr)
		{
			mpNearestGravityBody = pGBody;
			mNGB_DistSqr = thisDistSqr; 
		}
	}

	// If we did not find any...
	if (mpNearestGravityBody == nullptr)
	{
		mNGB_DistSqr = 0.0f;
		mNGB_Dist = 0.0f;
		isInGravity = false;
	}
	// If we found one...
	else
	{
		// Precompute distance.
		mNGB_Dist = sqrt(mNGB_DistSqr);
		isInGravity = true;

		// Other precomputations:
		calculateOrbitalParameters();
	}
}

void AIEnemyCore::calculateOrbitalParameters()
{
	if (mpNearestGravityBody == nullptr || mpPhysicsBody == nullptr)
		return;

	float const G_CONST = GlobalManager::getPhysicsManager()->universalGravitationalConstant;

	// Precompute relative position vectors.
	mNGB_RelativePosition = mpPhysicsBody->mPosition - mpNearestGravityBody->mPosition;
	mNGB_RelativePositionNormal = glm::vec2
	(
		mNGB_RelativePosition.y,
		-mNGB_RelativePosition.x
	);

	// Just going to call the gravitating body a planet for readability.
	glm::vec2 planetVelocity = mpNearestGravityBody->mVelocity;
	glm::vec2 myVelocity = mpPhysicsBody->mVelocity;

	// Find our relative velocity.
	mNGB_RelativeVelocity = myVelocity - planetVelocity;
	mNGB_RelativeSpeed = glm::length(mNGB_RelativeVelocity);

	// Find our orbital speed.
	mOrbitalSpeedAtCurrentAltitude = sqrt(
		G_CONST * mpNearestGravityBody->mMass
		/ (mNGB_Dist  * 1000.0f)
	);
	mOrbitalSpeedAtCurrentAltitude /= 1000.0f;

	// Find our prograde and radial velocities. Unfortunately pretty expensive.
	mVelocityPrograde = glm::proj(mNGB_RelativeVelocity, mNGB_RelativePositionNormal);
	mVelocityRadial = glm::proj(mNGB_RelativeVelocity, mNGB_RelativePosition);
}

void AIEnemyCore::keepOrbit(float closestDistSqr, PhysicsBody* pClosest)
{
	//std::cout << "Prograde:" << progradeVelocity.x << ", " << progradeVelocity.y << std::endl;
	//std::cout << "Radial:" << radialVelocity.x << ", " << radialVelocity.y << std::endl;
	//std::cout << "Orbital radius is " << orbitalRadius << " and desired altitiude is " << mDesiredAltitude << std::endl;

	float orbitalRadius = mNGB_Dist;
	// Go up or down.
	if (orbitalRadius > mDesiredAltitude + mOrbitThickness)
	{
		// Try go down
		// Cut our orbital velocity down a little.
		float desiredSpeed = mOrbitalSpeedAtCurrentAltitude * (1.0f - mOrbitalAdjustmentAgression);
		// Aim our velocity vector down a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(mNGB_RelativePosition) * mOrbitalSpeedAtCurrentAltitude * (0.0f - mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(mVelocityPrograde) * mOrbitalSpeedAtCurrentAltitude * (1.0f - mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		//std::cout << "dvr: " << desiredVelocityRadial.x << ", " << desiredVelocityRadial.y << std::endl;

		matchVelocityVector(desiredVelocityVector + pClosest->mVelocity);
	}
	else if (orbitalRadius < mDesiredAltitude - mOrbitThickness)
	{
		// Try go up
		// Increase our velocity a little
		float desiredSpeed = mOrbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		// Aim our velocity vector up a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(mNGB_RelativePosition) * mOrbitalSpeedAtCurrentAltitude * (0.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(mVelocityPrograde) * mOrbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		matchVelocityVector(desiredVelocityVector + pClosest->mVelocity);
	}
	else
	{
		// Try to maintain proper orbital velocity.
		float desiredSpeed = mOrbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(mVelocityPrograde) * mOrbitalSpeedAtCurrentAltitude;

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