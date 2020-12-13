/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			MissileLauncher.cpp
	Purpose				Implementations for the missile launcher component.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#include "MissileLauncher.h"
#include "../Managers/GlobalManager.h"

MissileLauncher::MissileLauncher() : Component(ComponentTypes::TYPE_MISSILELAUNCHER)
{
}

MissileLauncher::~MissileLauncher()
{
}


void MissileLauncher::Initialize()
{
	// Subscribe to target lock events. Will only be responded to if we're parented to the player.
	GlobalManager::getEventManager()->Subscribe(EventType::TARGET_LOCK, this->mpOwner);
}

void MissileLauncher::Update()
{
}

void MissileLauncher::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::TARGET_LOCK)
	{
		// Only respond if we're parented to the player.
		GameObject* pParent = mpOwner->getParent();
		if (pParent == nullptr || pParent->mName != "PLAYER")
			return;

		TargetLockEvent* pTargetEvent = static_cast<TargetLockEvent*>(pEvent);
		this->mTargetGOName = pTargetEvent->mTargetGOName;
	}
}


void MissileLauncher::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{



}