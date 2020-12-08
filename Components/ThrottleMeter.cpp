/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ThrottleMeter.cpp
	Purpose				Implementations for ThrottleMeter.h.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/26/2020

- End Header --------------------------------------------------------*/

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
		if (pShipDataEvent->mpShipData->mpOwner->mName.compare("PLAYER") == 0)
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
