#pragma once
#include "Component.h"

#include "glm/glm.hpp"

class Shape;

// Roughly equivalent to the example 'body.h'.
class PhysicsDynamic : public Component
{
public:
	PhysicsDynamic();
	~PhysicsDynamic();

	void Update();
	void Integrate(float deltaTme);

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
};

