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

	void Initialize();
	void Update();

	void Integrate(float deltaTime);

	void calculateGravityForces(); // Applies all the gravitational force vectors.
	void applyForce(glm::vec2 F);
	void applyTorque(float T);
	
	void setTimedIgnoreCollision(PhysicsBody * pIgnored, float ignoreTime);
	PhysicsBody* getIgnoredPhysicsBody() { return this->mpIgnoredPhysicsBody; };

	bool hasGravity() { return mHasGravity; }; // hasGravity is read-only. Setter needs to inform the physics manager.
	void enableGravity();
	void disableGravity();
	void setMass(float mass); // sets mass, calculates and sets invMass.

	void handleEvent(Event* pEvent);
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	glm::vec2 mForwardDir;
	glm::vec2 mRightDir;

	glm::vec2 mPosition;
	glm::vec2 mPrevPosition;
	glm::vec2 mVelocity;
	glm::vec2 mAcceleration;
	glm::vec2 mTotalForce;
	float mAngle, mPrevAngle, mAngularVelocity, mAngularAcceleration, mTotalTorque;
	float mMass, mInvMass;

	Shape* mpShape; 


private:
	// No private methods.
private:
	PhysicsBody* mpIgnoredPhysicsBody;
	float mIgnorePhysicsBodyTimer;
	bool mHasGravity; 
};

