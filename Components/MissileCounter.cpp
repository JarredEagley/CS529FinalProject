/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			MissileCounter.cpp
	Purpose				Implementations for MissileCounter.h.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/12/2020

- End Header --------------------------------------------------------*/

#include "MissileCounter.h"
#include "../Managers/GlobalManager.h"

MissileCounter::MissileCounter() : Component(ComponentTypes::TYPE_MISSILECOUNTER)
{
}

MissileCounter::~MissileCounter()
{
}


void MissileCounter::Initialize()
{
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, this->mpOwner);
}

void MissileCounter::Update() {}

void MissileCounter::handleEvent(Event* pEvent)
{
	if (mpGLText == nullptr)
	{
		mpGLText = static_cast<GLText*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLTEXT));
		return;
	}

	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);
		if (pShipDataEvent->mpShipData->mpOwner->mName == "PLAYER")
		{
			int missileCount = pShipDataEvent->mpShipData->mMissiles;
			
			std::string msg = "Nuclear Missiles: ";
			msg += std::to_string( missileCount);

			mpGLText->mTextContent = msg;

			if (missileCount > 0)
				mpGLText->mColor = glm::vec4(0.1, 0.9, 0.2, 1.0);
			else
				mpGLText->mColor = glm::vec4(0.9, 0.1, 0.1, 0.5);
		}
	}
}


void MissileCounter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}