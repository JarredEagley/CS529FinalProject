/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ShipData.h
	Purpose:			Contains all the data needed for a space ship, such
						as acceleration and health. Also holds the ship's
						physics body and relays movement commands. This
						way, ship movement isn't inherently tied to whatever
						is commanding it; be it player or ai.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/25/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "PhysicsBody.h"

class ShipData : public Component
{
public:
	ShipData();
	~ShipData();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	void throttleUp();
	void throttleDown();
	void setThrottle(float throttle);
	
	void takeDamage(float dmg);
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
	float mThrottleDeadzone; // Set this to zero for the throttle to be instantaneous.
	float mThrottleSensitivity;

	float mAngularDamping; // How agressively ship will try to stop spinning without input.

	PhysicsBody* mpPhysicsBody;

private:
	// No private methods.
private:
	glm::vec2 mThrustInput;
	float mRotationInput;
};

