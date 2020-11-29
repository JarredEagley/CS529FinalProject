#include "FuelMeter.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"
#include "ShipData.h"

FuelMeter::FuelMeter() : Component(ComponentTypes::TYPE_METER_FUEL),
mFuel(0.0f);
{}

FuelMeter::~FuelMeter()
{}


void FuelMeter::Initialize()
{
	// We want to know when shipdata updates.
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, mpOwner);
}

void FuelMeter::Update() {}

void FuelMeter::setUniformData(ShaderProgram* pProgram)
{
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "fillLevel");
	glUniform1f(loc, mFuel);
}


void FuelMeter::handleEvent(Event* pEvent)
{
	// Listen for shipdata updates.
	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);
		this->mFuel = pShipDataEvent->mpShipData->mFuel;
	}
}


void FuelMeter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}
