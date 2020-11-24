#pragma once
#include "Component.h"

#include "glm/glm.hpp"

class Shape;

// Roughly equivalent to the example 'body.h'.
class PhysicsBody : public Component
{
public:
	PhysicsBody();
	~PhysicsBody();

	void Update();
	void Integrate(float deltaTme);

	bool hasGravity() { return mHasGravity; }; // hasGravity is read-only. Setter needs to inform the physics manager.
	void enableGravity();
	void disableGravity();

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	glm::vec2 mPosition;
	glm::vec2 mPrevPosition;
	glm::vec2 mVelocity;
	glm::vec2 mPrevVelocity;
	glm::vec2 mAcceleration;
	glm::vec2 mPrevAccleration;
	glm::vec2 mTotalForce;
	float mAngle, mPrevAngle, mAngularVelocity, mPrevAngularVelocity, mAngularAcceleration, mPrevAngularAcceleration, mTotalTorque;
	float mMass, mInvMass;

	Shape* mpShape; 

private:
private:
	bool mHasGravity; 
};

