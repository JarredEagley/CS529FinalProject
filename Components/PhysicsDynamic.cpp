#include "PhysicsDynamic.h"
#include "Transform.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "../Managers/GlobalManager.h"

PhysicsDynamic::PhysicsDynamic() : Component(ComponentTypes::TYPE_PHYSICSDYNAMIC)
{
	mPosition = glm::vec2(0.0f);
	mAngle = 0.0f;
	mPrevPosition = glm::vec2(0.0f);
	mPrevAngle = 0.0f;
	mVelocity = glm::vec2(0.0f);
	mAngularVelocity = 0.0f;
	mAcceleration = glm::vec2(0.0f);
	mAngularAcceleration = 0.0f;
	mTotalForce = glm::vec2(0.0f);
	mTotalTorque = 0.0f;

	mMass = mInvMass = 1.0f;

	mpShape = nullptr;
}

PhysicsDynamic::~PhysicsDynamic()
{
	if (mpShape != nullptr)
		delete mpShape;
}


void PhysicsDynamic::Update()
{}


void PhysicsDynamic::Integrate(float deltaTime)
{
	// Get the associated transform. 
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // Probably shouldn't get this EVERY integration. Oh well.

	if (nullptr != pT)
	{
		mPosition = glm::vec2(pT->getPosition());
		mAngle = pT->getRotation();
	}

	mPrevPosition = mPosition;
	mPrevAngle = mAngle;

	// Compute acceleration.
	// Normally global gravity would be here on this line.
	mAcceleration = mTotalForce * mInvMass;
	mAngularAcceleration = mTotalTorque * mInvMass;

	mTotalForce = glm::vec2(0.0f);
	mTotalTorque = 0.0f;

	// Integrate velocity. V1 = a*t + V0
	mVelocity = mAcceleration * deltaTime + mVelocity;
	mAngularVelocity = mAngularAcceleration * deltaTime + mAngularVelocity;

	// Integrate position. P1 = V1*t + P0
	mPosition = mVelocity * deltaTime + mPosition;
	mAngle = mAngularVelocity * deltaTime + mAngle;

	if (nullptr != pT)
	{
		pT->setPosition(mPosition);
		pT->setRotation(mAngle);
	}
}


void PhysicsDynamic::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Read in mMass.

	// Store mInvMass.

	// Read in shapename.

	// If circle read in a radius.

	// If AABB read in a left right top bottom.




}