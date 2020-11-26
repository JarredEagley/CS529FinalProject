#pragma once
#include "Component.h"
#include "ComponentTypes.h"

class ShipData : public Component
{
public:
	ShipData();
	~ShipData();

	void Update();

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	float mHealth; // ie structural integrity.
	float mFuel;
	int mCoilBullets;
	int mMissiles;

	float mMainAcceleration;		// Main drive
	float mSecondaryAcceleration;	// Maneuvering thrusters
	float mAngularAcceleration;		// RCS

	float fuelEfficiency;	// Basically ISP, but without the real-world math.

private:
private:
};

