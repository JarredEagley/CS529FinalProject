
#include "AIEnemyCore.h"
#include "../Managers/GlobalManager.h"

#include "glm/gtx/projection.hpp"

AIEnemyCore::AIEnemyCore() : Component(ComponentTypes::TYPE_AI_CORE),
mpPhysicsBody(nullptr), mpShipData(nullptr)
{}

AIEnemyCore::~AIEnemyCore()
{
	// Remove from the enemies list.
	// Slightly mroe complex than expected to account for duplicates during erasure.
	GameStateManager* pGSM = GlobalManager::getGameStateManager();
	auto findElement = std::find(pGSM->mLivingEnemies.begin(), pGSM->mLivingEnemies.end(), this->mpOwner->mName);
	if (findElement != pGSM->mLivingEnemies.end())
	{
		pGSM->mLivingEnemies.erase(findElement);
	}
	//GlobalManager::getGameStateManager()->mLivingEnemies.remove(this->mpOwner->mName);
}

void AIEnemyCore::Initialize()
{
	// Register with the enemies list.
	GlobalManager::getGameStateManager()->mLivingEnemies.push_back(this->mpOwner->mName);

	mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
	mpPhysicsBody = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
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

void AIEnemyCore::handleEvent(Event* pEvent) {}

// AI Functions

void AIEnemyCore::getNearestGravityBody()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;
	if (mpShipData == nullptr || mpPhysicsBody == nullptr || !mpPhysicsBody->mHasRunOnce )
		return;
	if (!mpOwner->mIsAlive)
		return;

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
	mOrbitalVelocityPrograde = glm::proj(mNGB_RelativeVelocity, mNGB_RelativePositionNormal);
	mOrbitalVelocityRadial = glm::proj(mNGB_RelativeVelocity, mNGB_RelativePosition);
}

//void AIEnemyCore::keepOrbit(float closestDistSqr, PhysicsBody* pClosest)
void AIEnemyCore::keepOrbit()
{
	if (mpNearestGravityBody == nullptr || mpPhysicsBody == nullptr || mpShipData == nullptr)
		return;

	float orbitalRadius = mNGB_Dist;
	// Go up or down.
	if (orbitalRadius > mDesiredAltitude + mOrbitThickness)
	{
		// Try go down
		// Cut our orbital velocity down a little.
		float desiredSpeed = mOrbitalSpeedAtCurrentAltitude * (1.0f - mOrbitalAdjustmentAgression);
		// Aim our velocity vector down a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(mNGB_RelativePosition) * mOrbitalSpeedAtCurrentAltitude * (0.0f - mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(mOrbitalVelocityPrograde) * mOrbitalSpeedAtCurrentAltitude * (1.0f - mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		//std::cout << "dvr: " << desiredVelocityRadial.x << ", " << desiredVelocityRadial.y << std::endl;

		matchVelocityVector(desiredVelocityVector + mpNearestGravityBody->mVelocity);
	}
	else if (orbitalRadius < mDesiredAltitude - mOrbitThickness)
	{
		// Try go up
		// Increase our velocity a little
		float desiredSpeed = mOrbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		// Aim our velocity vector up a little.
		glm::vec2 desiredVelocityRadial = glm::normalize(mNGB_RelativePosition) * mOrbitalSpeedAtCurrentAltitude * (0.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(mOrbitalVelocityPrograde) * mOrbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityVector = desiredVelocityRadial + desiredVelocityPrograde;

		matchVelocityVector(desiredVelocityVector + mpNearestGravityBody->mVelocity);
	}
	else
	{
		// Try to maintain proper orbital velocity.
		float desiredSpeed = mOrbitalSpeedAtCurrentAltitude * (1.0f + mOrbitalAdjustmentAgression);
		glm::vec2 desiredVelocityPrograde = glm::normalize(mOrbitalVelocityPrograde) * mOrbitalSpeedAtCurrentAltitude;

		matchVelocityVector(desiredVelocityPrograde + mpNearestGravityBody->mVelocity);
	}
}


void AIEnemyCore::matchVelocityVector(glm::vec2 desiredVelocity)
{
	if (mpPhysicsBody == nullptr || mpShipData == nullptr)
		return;

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

// Best used when theres no gravity.
void AIEnemyCore::tryToStop()
{
	if (mpShipData == nullptr || mpPhysicsBody == nullptr)
		return;

	glm::vec2 myVelocity = mpPhysicsBody->mVelocity;

	glm::vec2 accelVector = glm::normalize(myVelocity);
	glm::vec2 accelNormal = glm::vec2(accelVector.y, -accelVector.x);

	float mySpeed = glm::length(myVelocity);

	// Use drive if speed above a threshold and fuel not depleted.
	if (mySpeed > mManeuveringSpeedThreshold && mpShipData->mFuel > 0.1f)
	{
		// Align
		float alignmentAmount = alignToVector(accelVector);

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

	// Use maneuvering thrusters too regardless.
	mpShipData->applyThrustSecondary(-accelVector);
}

float AIEnemyCore::alignToVector(glm::vec2 alignmentVector)
{
	if (mpPhysicsBody == nullptr || mpShipData == nullptr)
		return 0.0f;

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
	if (mpPhysicsBody == nullptr || mpShipData == nullptr)
		return;

	if (mOrientationBehavior == "FACE_PLAYER")
	{
		GameObject* pPlayer = GlobalManager::getGameObjectManager()->getGameObject("PLAYER");
		if (pPlayer == nullptr)
			return;
		PhysicsBody* pPlayerBody = static_cast<PhysicsBody*>(pPlayer->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
		if (pPlayerBody == nullptr)
			return;

		glm::vec2 alignmentVec = this->mpPhysicsBody->mPosition - pPlayerBody->mPosition;
		alignmentVec = glm::normalize(alignmentVec);

		this->alignToVector(alignmentVec);
	}
	else if (mOrientationBehavior == "FACE_PROGRADE")
	{
		this->alignToVector(glm::normalize(-mOrbitalVelocityPrograde));
	}
	else if (mOrientationBehavior == "FACE_RADIAL")
	{
		this->alignToVector(glm::normalize(-mOrbitalVelocityRadial));
	}
	// By default, no need to do anything else; just use the natural angular damping specified in ship data.
}


void AIEnemyCore::calculatePlayerParameters()
{
	if (mpPhysicsBody == nullptr)
		return;

	GameObject* pPlayerGO = GlobalManager::getGameObjectManager()->getGameObject("PLAYER");
	if (pPlayerGO == nullptr)
		return;

	PhysicsBody* pPlayerBody = static_cast<PhysicsBody*>(pPlayerGO->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	if (pPlayerBody == nullptr)
		return;

	mPlayer_RelativePosition = mpPhysicsBody->mPosition - pPlayerBody->mPosition;
	mPlayer_DistanceSqr =
		(mPlayer_RelativePosition.x * mPlayer_RelativePosition.x)
		+ (mPlayer_RelativePosition.y * mPlayer_RelativePosition.y);
	
	mPlayer_Distance = sqrt(mPlayer_DistanceSqr);

	mPlayer_RelativePositionNormal = glm::vec2(mPlayer_RelativePosition.y, -mPlayer_RelativePosition.x);

	mPlayer_RelativeVelocity = mpPhysicsBody->mVelocity - pPlayerBody->mVelocity;

	mPlayer_OrthogonalVelocity = glm::vec2(mPlayer_RelativeVelocity.y, -mPlayer_RelativeVelocity.x); 
}


glm::vec2 AIEnemyCore::getTurretAimAngle()
{
	// Temp
	return mPlayer_RelativePosition;

}



void AIEnemyCore::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Desired Altitude") && inputObj["Desired Altitude"].IsNumber())
	{
		this->mDesiredAltitude = inputObj["Desired Altitude"].GetFloat();
	}

	if (inputObj.HasMember("Shoot Range") && inputObj["Shoot Range"].IsNumber())
	{
		this->mShootRange = inputObj["Shoot Range"].GetFloat();
	}

	if (inputObj.HasMember("Missile Launch Range") && inputObj["Missile Launch Range"].IsNumber())
	{
		this->mMissileLaunchRange = inputObj["Missile Launch Range"].GetFloat();
	}

	if (inputObj.HasMember("Missile Launch Probability") && inputObj["Missile Launch Probability"].IsNumber())
	{
		this->mMissileLaunchProbability = inputObj["Missile Launch Probability"].GetFloat();
	}

	if (inputObj.HasMember("Missile Launch Time") && inputObj["Missile Launch Time"].IsNumber())
	{
		this->mMissileLaunchTimerMax = inputObj["Missile Launch Time"].GetFloat();
		this->mMissileLaunchTimer = mMissileLaunchTimerMax;
	}


	if (inputObj.HasMember("Orientation Behavior") && inputObj["Orientation Behavior"].IsString())
	{
		this->mOrientationBehavior = inputObj["Orientation Behavior"].GetString();
	}

	if (inputObj.HasMember("Maneuvering Speed Threshold") && inputObj["Maneuvering Speed Threshold"].IsNumber())
	{
		this->mManeuveringSpeedThreshold = inputObj["Maneuvering Speed Threshold"].GetFloat();
	}

	if (inputObj.HasMember("Orbital Adjustment Agression") && inputObj["Orbital Adjustment Agression"].IsNumber())
	{
		this->mOrbitalAdjustmentAgression = inputObj["Orbital Adjustment Agression"].GetFloat();
	}

	if (inputObj.HasMember("Orbit Thickness") && inputObj["Orbit Thickness"].IsNumber())
	{
		this->mOrbitThickness = inputObj["Orbit Thickness"].GetFloat();
	}


}