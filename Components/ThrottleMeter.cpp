#include "ThrottleMeter.h"

#include "ShipData.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

ThrottleMeter::ThrottleMeter() : Component(ComponentTypes::TYPE_MARKER_THROTTLE),
mpTransform(nullptr), mpParentTransform(nullptr)
{
}

ThrottleMeter::~ThrottleMeter()
{
}


void ThrottleMeter::Initialize() 
{
	// Subscrie my GameObject to shipdata updates.
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, mpOwner);
}

void ThrottleMeter::Update()
{
}


void ThrottleMeter::handleEvent(Event* pEvent)
{
	// Get transforms if we don't have them.
	if (mpTransform == nullptr || mpParentTransform == nullptr)
	{
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		mpParentTransform = static_cast<Transform*>(mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	// Listen for shipdata updates.
	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);

		// Check if it's the player ship.
		// Note: Player ship must always be named 'PLAYER'
		if (pShipDataEvent->mpShipData->mpOwner->mName == "PLAYER")
		{
			glm::vec3 parentScale = mpParentTransform->getScale();

			mpTransform->setY(
				(pShipDataEvent->mpShipData->mThrottle / 100.0f) // Throttle
				* 0.9 // Scale to account for the offset
				- (parentScale.y / 2.5) // Offset
			);

			//mpTransform->setY( ( (pShipDataEvent->mpShipData->mThrottle * parentScale.y) / GlobalManager::getGraphicsManager()->mWindowHeight ) - (parentScale.y/2.5f) );
		}
	}
}


void ThrottleMeter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
