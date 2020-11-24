#include "PhysicsBody.h"
#include "Transform.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "../Managers/GlobalManager.h"

PhysicsBody::PhysicsBody() : Component(ComponentTypes::TYPE_PHYSICSBODY)
{
	hasGravity = false;
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

PhysicsBody::~PhysicsBody()
{
	if (mpShape != nullptr)
		delete mpShape;
}


void PhysicsBody::Update()
{}


void PhysicsBody::Integrate(float deltaTime)
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


void PhysicsBody::enableGravity()
{
	if (this->hasGravity)
		return;

	// Enable gravity and push this physics body onto the physics manager's vector.
	// TO-DO: Handle this physics body getting destroyed; needs to be removed from the physics manager.
	this->hasGravity = true;
	GlobalManager::getPhysicsManager()->gravityBodies.push_back(this);
}

void PhysicsBody::disableGravity()
{
	if (!this->hasGravity)
		return;
	// Disable gravity and remove this physics body from the physics manager's vector.
	this->hasGravity = false;
	GlobalManager::getPhysicsManager()->gravityBodies.remove(this); // TO-DO: A std::list may be more appropriate for gravityBodies... // TO-DO: Will this call this body's destructor by accident???
}

void PhysicsBody::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Read in mMass.

	// Store mInvMass.

	// Read in shapename.

	// If circle read in a radius.

	// If AABB read in a left right top bottom.

}