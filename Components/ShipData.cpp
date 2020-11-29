#include "ShipData.h"
#include <iostream>
#include "../Managers/GlobalManager.h"

ShipData::ShipData() : Component(ComponentTypes::TYPE_SHIPDATA),
mHealth(100.0f), mMaxHealth(100.0f), mPower(100.0f), mMaxPower(100.0f), mPowerProduction(1.0f),
mFuel(0.0f), mMaxFuel(0.0f), mCoilBullets(0), mMissiles(0),
mMainAcceleration(0.0f), mSecondaryAcceleration(0.0f), mAngularAcceleration(0.0f),
mFuelEfficiency(0.0f),
mThrottle(0.0f), mThrottleSensitivity(1.0f),
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

	// Broadcast ship data to those listening for it. IE fuel meter, health bar.
	ShipDataUpdatedEvent* pShipDataEvent = new ShipDataUpdatedEvent(this);
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pShipDataEvent);
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


// Should be called once per update while accelerating with the main drive.
void ShipData::useFuel()
{
	mFuel -= GlobalManager::getFrameRateController()->getFrameTimeSec() / mFuelEfficiency;
}

void ShipData::usePower( float pwr )
{
	mPower -= GlobalManager::getFrameRateController()->getFrameTimeSec() * pwr;
}

void ShipData::generatePower()
{
	mPower += GlobalManager::getFrameRateController()->getFrameTimeSec() * mPowerProduction;
}


void ShipData::applyThrustMain()
{
	if (mpPhysicsBody == nullptr)
		return;

	//mpPhysicsBody->applyForce(mpPhysicsBody->mForwardDir * mMainAcceleration * (mThrottle / 100.0f));
	
	useFuel();
}

void ShipData::applyThrustSecondary(glm::vec2 input)
{
	if (mpPhysicsBody == nullptr)
		return;

	if (input == glm::vec2(0.0f))
		return;

	// mForwardDir = glm::vec2(sin(glm::radians(-mAngle)), cos(glm::radians(mAngle)));
	// mRightDir = glm::vec2(cos(glm::radians(mAngle)), sin(glm::radians(mAngle)));
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
		std::cout << "Warning: ShipData 'Health' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Health Max") && inputObj["Health Max"].IsNumber())
	{
		this->mMaxHealth = inputObj["Health Max"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Health Max' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Power") && inputObj["Power"].IsNumber())
	{
		this->mPower = inputObj["Power"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Power' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Power Max") && inputObj["Power Max"].IsNumber())
	{
		this->mMaxPower = inputObj["Power Max"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Power Max' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Power Production") && inputObj["Power Production"].IsNumber())
	{
		this->mPowerProduction = inputObj["Power Production"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Power Production' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Fuel") && inputObj["Fuel"].IsNumber())
	{
		this->mFuel = inputObj["Fuel"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Fuel' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Fuel Max") && inputObj["Fuel Max"].IsNumber())
	{
		this->mMaxFuel = inputObj["Fuel Max"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Fuel Max' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Fuel Efficiency") && inputObj["Fuel Efficiency"].IsNumber())
	{
		this->mFuelEfficiency = inputObj["Fuel Efficiency"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Fuel Efficiency' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Coil Bullets") && inputObj["Coil Bullets"].IsNumber())
	{
		this->mCoilBullets = inputObj["Coil Bullets"].GetInt(); // Integer, not float.
	}
	else
		std::cout << "Warning: ShipData 'Coil Bullets' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Missiles") && inputObj["Missiles"].IsNumber())
	{
		this->mMissiles = inputObj["Missiles"].GetInt(); // Integer, not float.
	}
	else
		std::cout << "Warning: ShipData 'Missiles' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Main Acceleration") && inputObj["Main Acceleration"].IsNumber())
	{
		this->mMainAcceleration = inputObj["Main Acceleration"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Main Acceleration' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Secondary Acceleration") && inputObj["Secondary Acceleration"].IsNumber())
	{
		this->mSecondaryAcceleration = inputObj["Secondary Acceleration"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Secondary Acceleration' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Angular Acceleration") && inputObj["Angular Acceleration"].IsNumber())
	{
		this->mAngularAcceleration = inputObj["Angular Acceleration"].GetFloat();
	}
	else
		std::cout << "Warning: ShipData 'Angular Acceleration' parameter was missing or improperly formatted. Default used." << std::endl;


	if (inputObj.HasMember("Throttle Sensitivity") && inputObj["Throttle Sensitivity"].IsNumber())
	{
		this->mThrottleSensitivity = inputObj["Throttle Sensitivity"].GetFloat();
	}
		std::cout << "Warning: ShipData 'Throttle Sensitivity' parameter was missing or improperly formatted. Default used." << std::endl;

}