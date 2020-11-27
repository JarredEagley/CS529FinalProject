#include "ShipData.h"
#include <iostream>
#include "../Managers/GlobalManager.h"

ShipData::ShipData() : Component(ComponentTypes::TYPE_SHIPDATA),
mHealth(100.0f), mMaxHealth(100.0f), mPower(100.0f), mMaxPower(100.0f), mPowerProduction(1.0f),
mFuel(0.0f), mMaxFuel(0.0f), mCoilBullets(0), mMissiles(0),
mMainAcceleration(0.0f), mSecondaryAcceleration(0.0f), mAngularAcceleration(0.0f),
fuelEfficiency(0.0f)
{
}

ShipData::~ShipData()
{
}


void ShipData::Update()
{
	// Broadcast ship data to those listening for it. IE fuel meter, health bar.
	ShipDataUpdatedEvent* pShipDataEvent = new ShipDataUpdatedEvent(this);
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pShipDataEvent);
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
		this->fuelEfficiency = inputObj["Fuel Efficiency"].GetFloat();
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





}