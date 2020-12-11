/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FXPlume.cpp
	Purpose				Implementations for FXPlume.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/10/2020

- End Header --------------------------------------------------------*/

#include "FXPlume.h"
#include "../Managers/GlobalManager.h"
#include "ShipData.h"

FXPlume::FXPlume() : Component(ComponentTypes::TYPE_FX_PLUME)
{
}

FXPlume::~FXPlume()
{
}


void FXPlume::Initialize()
{
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, mpOwner);
}

void FXPlume::Update()
{
}

void FXPlume::handleEvent(Event* pEvent)
{
	// Make sure we have a parent GO.
	GameObject* pParentGO = mpOwner->getParent();
	if (pParentGO == nullptr)
		return;

	// Handle shipdata update events.
	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		// Make sure this event is from parent GameObject.
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);
		if (pShipDataEvent->mpShipData->mpOwner == pParentGO)
		{
			// Make sure we've got a graphics component.
			if (pGLRect == nullptr)
			{
				pGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
				return;
			}

			// Set my alpha based on throttle and fuel.
			if (pShipDataEvent->mpShipData->mFuel > 0.0f)
				pGLRect->setAlpha(pShipDataEvent->mpShipData->mThrottle / 100.0f);
			else
				pGLRect->setAlpha(0.0f);
		}
	}
}


void FXPlume::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}