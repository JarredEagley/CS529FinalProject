#include "FuelMeter.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

FuelMeter::FuelMeter() : Component(ComponentTypes::TYPE_METER_FUEL)
{}

FuelMeter::~FuelMeter()
{}


void FuelMeter::Initialize()
{
	// We want to know when shipdata updates.
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, mpOwner);
}

void FuelMeter::Update()
{}


void FuelMeter::handleEvent(Event* pEvent)
{
}


void FuelMeter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}
