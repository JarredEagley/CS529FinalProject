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
	if (mpGLRect == nullptr)
	{
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		return;
	}

	GameObject* pParentGO = mpOwner->getParent();
	if (pParentGO == nullptr)
		return;

	GLRect* pParentRect = static_cast<GLRect*>(pParentGO->GetComponent(ComponentTypes::TYPE_GLRECT));
	if (pParentRect == nullptr)
		return;

	// Mirror parent color, like turrets.
	mpGLRect->setColor(pParentRect->getColor());

	// -- launching -- //

	// Launch hotkey is spacebar.
	if (GlobalManager::getInputManager()->IsKeyTriggered(SDL_SCANCODE_SPACE))
	{
		if (mTargetGOName == "")
			return;

		GameObject* pTargetGO = GlobalManager::getGameObjectManager()->getGameObject(mTargetGOName);
		if (pTargetGO == nullptr)
			return;

		// We have a valid target. Fire the missile!

	}
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
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Launch Force") && inputObj["Launch Force"].IsArray() && inputObj["Launch Force"].GetArray().Size() >= 2)
	{
		auto arr = inputObj["Launch Force"].GetArray();

		if (arr[0].IsNumber() && arr[1].IsNumber())
		{
			this->mLaunchForce = glm::vec2(
				arr[0].GetFloat(),
				arr[1].GetFloat()
			);
		}
		else
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Missile Launcher launch force was improperly formatted. Default used." << std::endl;
		}
	}


}