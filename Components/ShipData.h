#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "PhysicsBody.h"

class ShipData : public Component
{
public:
	ShipData();
	~ShipData();

	void Update();

	void throttleUp();
	void throttleDown();
	void setThrottle(float throttle);
	
	void useFuel(); // Uses fuel inverse to efficiency.
	void usePower(float pwr);
	void generatePower();

	void applyThrustMain();
	void applyThrustSecondary(glm::vec2 input); // Converts input to local coordinates and scales by secondary thrust factor.
	void applySpin(float scaleFactor = 1.0f);
	void applyAngularDamping();

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	float mHealth; // ie structural integrity.
	float mMaxHealth;
	float mPower;
	float mMaxPower;
	float mPowerProduction;

	float mFuel;
	float mMaxFuel;
	float mFuelEfficiency;	// Basically ISP, but without the real-world math.

	int mCoilBullets;
	int mMissiles;

	float mMainAcceleration;		// Main drive
	float mSecondaryAcceleration;	// Maneuvering thrusters
	float mAngularAcceleration;		// RCS

	float mThrottle; // Throttle
	float mThrottleSensitivity;

	float mAngularDamping; // How agressively ship will try to stop spinning without input.

	PhysicsBody* mpPhysicsBody;

private:
	// No private methods.
private:
	glm::vec2 mThrustInput;
	float mRotationInput;
};

