#include "ThrottleMeter.h"

#include "ShipData.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

ThrottleMeter::ThrottleMeter() : Component(ComponentTypes::TYPE_MARKER_THROTTLE)
{
}

ThrottleMeter::~ThrottleMeter()
{
}


void ThrottleMeter::Initialize() 
{
	// Subscrie my GameObject to shipdata updates.
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, this->mpOwner);
}

void ThrottleMeter::Update()
{

}


void ThrottleMeter::handleEvent(Event* pEvent)
{
	// Get transform if we don't have it.
	if (mpTransform == nullptr)
	{
		mpTransform = static_cast<Transform*>(this->mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	}

	// Listen for shipdata updates.
	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);

		// Check if it's the player ship.
		// Note: Player ship must always be named 'PLAYER'
		if (pShipDataEvent->mpShipData->mpOwner->mName == "PLAYER")
		{
			//std::cout << "DEBUG - Getting ship data updates from player success.";
			this->mpTransform->setY(pShipDataEvent->mpShipData->mThrottle);
		}
	}
}


void ThrottleMeter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
