#include "ThrottleMeter.h"
#include "ShipData.h"
#include "../Managers/GlobalManager.h"

ThrottleMeter::ThrottleMeter() : Component(ComponentTypes::TYPE_METER_THROTTLE)
{
}

ThrottleMeter::~ThrottleMeter()
{
}


void ThrottleMeter::Update()
{

}


void ThrottleMeter::handleEvent(Event* pEvent)
{
	// Listen for shipdata updates.
	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);

		// Check if it's the player ship.
		// Note: Player ship must always be named 'PLAYER'
		if (pShipDataEvent->mpShipData->mpOwner->mName == "PLAYER")
		{
			std::cout << "DEBUG - Getting ship data updates from player success.";
		}
	}
}


void ThrottleMeter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Subscrie my GameObject to shipdata updates.
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, this->mpOwner);
}
