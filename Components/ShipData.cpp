/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ShipData.cpp
	Purpose:			Implementations for ShipData.h.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/25/2020

- End Header --------------------------------------------------------*/

#include "ShipData.h"
#include <iostream>
#include "../Managers/GlobalManager.h"

ShipData::ShipData() : Component(ComponentTypes::TYPE_SHIPDATA),
mHealth(100.0f), mMaxHealth(100.0f), mPower(100.0f), mMaxPower(100.0f), mPowerProduction(1.0f),
mFuel(0.0f), mMaxFuel(0.0f), mCoilBullets(0), mMissiles(0),
mMainAcceleration(0.0f), mSecondaryAcceleration(0.0f), mAngularAcceleration(0.0f),
mFuelEfficiency(0.0f),
mThrottle(0.0f), mThrottleDeadzone(0.1f), mThrottleSensitivity(1.0f),
mThrustInput(glm::vec3(0.0f)), mRotationInput(0.0f),
mAngularDamping(5.0f),
mpPhysicsBody(nullptr)
{
}

ShipData::~ShipData()
{
}


void ShipData::Initialize() {}

void ShipData::Update()
{
	// Try to get physics component.
	mpPhysicsBody = static_cast<PhysicsBody*>(mpOwner->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	if (mpPhysicsBody == nullptr)
	{
		std::cout << "Warning: Ship data component failed to find its physics body component." << std::endl;
		return;
	}

	// Main thrust indirectly controlled via throttle. Update it here.
	applyThrustMain();

	// Broadcast ship data to those listening for it. IE fuel meter, health bar.
	ShipDataUpdatedEvent* pShipDataEvent = new ShipDataUpdatedEvent(this);
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pShipDataEvent);
}

void ShipData::handleEvent(Event* pEvent)
{
	// Use collision events to apply damage.
	if (pEvent->mType == EventType::DO_DAMAGE)
	{
		DoDamageEvent* pDamageEvent = static_cast<DoDamageEvent*>(pEvent);
		this->takeDamage(pDamageEvent->mDamage);
		
	}
}


void ShipData::throttleUp()
{
	mThrottle = std::min(mThrottle + mThrottleSensitivity, 100.0f);
}

void ShipData::throttleDown()
{
	mThrottle = std::max(mThrottle - mThrottleSensitivity, 0.0f);
}

void ShipData::setThrottle(float throttle)
{
	mThrottle = throttle;
	mThrottle = std::min(mThrottle, 100.0f);
	mThrottle = std::max(mThrottle, 0.0f);
}


void ShipData::takeDamage(float dmg)
{
	this->mHealth -= dmg;
	if (this->mHealth < 0.0f)
	{
		// TO-DO: DESTROY!
	}
}

// Should be called once per update while accelerating with the main drive.
void ShipData::useFuel()
{
	mFuel -= (GlobalManager::getPhysicsManager()->getGameTime() * (mThrottle/100.0f) ) / mFuelEfficiency;
}

void ShipData::usePower( float pwr )
{
	mPower -= GlobalManager::getPhysicsManager()->getGameTime() * pwr;
}

void ShipData::generatePower()
{
	mPower += GlobalManager::getPhysicsManager()->getGameTime() * mPowerProduction;
}


void ShipData::applyThrustMain()
{
	if (mpPhysicsBody == nullptr)
		return;

	if (mThrottle < mThrottleDeadzone)
		return;

	if (mFuel > 0.0f)
	{
		mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mMainAcceleration * (mThrottle / 100.0f));
	}

	
	useFuel();
}

void ShipData::applyThrustSecondary(glm::vec2 input)
{
	if (mpPhysicsBody == nullptr)
		return;

	if (input == glm::vec2(0.0f))
		return;

	float a = mpPhysicsBody->mAngle;
	glm::vec2 thrustResult = glm::vec2(
		cos(glm::radians(a))*input.x - sin(glm::radians(a))*input.y,
		sin(glm::radians(a))*input.x + cos(glm::radians(a))*input.y
	);
	thrustResult = glm::vec2(-thrustResult.y, thrustResult.x);

	thrustResult *= mSecondaryAcceleration; 

	mpPhysicsBody->applyForce(thrustResult);

	// usePower( something );
}

void ShipData::applySpin(float scaleFactor)
{
	if (mpPhysicsBody == nullptr)
		return;

	mpPhysicsBody->applyTorque(mAngularAcceleration * scaleFactor);
}

void ShipData::applyAngularDamping()
{
	if (mpPhysicsBody == nullptr)
		return;

	// Uses ship's angular velocity and damping factor to try to slow down.
	applySpin(-mpPhysicsBody->mAngularVelocity * mAngularDamping);
}


void ShipData::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	rapidjson::GenericObject<true, rapidjson::Value> inputObj = inputMemberIt->value.GetObject();


	if (inputObj.HasMember("Health") && inputObj["Health"].IsNumber())
	{
		this->mHealth = inputObj["Health"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Health' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Health Max") && inputObj["Health Max"].IsNumber())
	{
		this->mMaxHealth = inputObj["Health Max"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Health Max' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Power") && inputObj["Power"].IsNumber())
	{
		this->mPower = inputObj["Power"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Power' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Power Max") && inputObj["Power Max"].IsNumber())
	{
		this->mMaxPower = inputObj["Power Max"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Power Max' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Power Production") && inputObj["Power Production"].IsNumber())
	{
		this->mPowerProduction = inputObj["Power Production"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Power Production' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Fuel") && inputObj["Fuel"].IsNumber())
	{
		this->mFuel = inputObj["Fuel"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Fuel' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Fuel Max") && inputObj["Fuel Max"].IsNumber())
	{
		this->mMaxFuel = inputObj["Fuel Max"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Fuel Max' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Fuel Efficiency") && inputObj["Fuel Efficiency"].IsNumber())
	{
		this->mFuelEfficiency = inputObj["Fuel Efficiency"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Fuel Efficiency' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Coil Bullets") && inputObj["Coil Bullets"].IsNumber())
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)

		this->mCoilBullets = inputObj["Coil Bullets"].GetInt(); // Integer, not float.
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Coil Bullets' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Missiles") && inputObj["Missiles"].IsNumber())
	{
		this->mMissiles = inputObj["Missiles"].GetInt(); // Integer, not float.
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Missiles' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Main Acceleration") && inputObj["Main Acceleration"].IsNumber())
	{
		this->mMainAcceleration = inputObj["Main Acceleration"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Main Acceleration' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Secondary Acceleration") && inputObj["Secondary Acceleration"].IsNumber())
	{
		this->mSecondaryAcceleration = inputObj["Secondary Acceleration"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Secondary Acceleration' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Angular Acceleration") && inputObj["Angular Acceleration"].IsNumber())
	{
		this->mAngularAcceleration = inputObj["Angular Acceleration"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Angular Acceleration' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Throttle Sensitivity") && inputObj["Throttle Sensitivity"].IsNumber())
	{
		this->mThrottleSensitivity = inputObj["Throttle Sensitivity"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Throttle Sensitivity' parameter was missing or improperly formatted. Default used." << std::endl;
			

	if (inputObj.HasMember("Throttle Deadzone") && inputObj["Throttle Sensitivity"].IsNumber())
	{
		this->mThrottleSensitivity = inputObj["Throttle Deadzone"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: ShipData 'Throttle Deadzone' parameter was missing or improperly formatted. Default used." << std::endl;

}