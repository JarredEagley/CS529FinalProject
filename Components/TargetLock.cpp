/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			TargetLock.cpp
	Purpose:			Implementations for TargetLock.h.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#include "TargetLock.h"

TargetLock::TargetLock() : Component(ComponentTypes::TYPE_TARGETLOCK) 
{
}

TargetLock::~TargetLock()
{
}


void TargetLock::Initialize()
{
}

void TargetLock::Update()
{
	if (mpTransform == nullptr || mpGLRect == nullptr)
	{
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		return;
	}

	// If player presses T for TARGET...
	if (GlobalManager::getInputManager()->IsKeyTriggered(SDL_SCANCODE_T))
	{
		getNextTarget();
	}

	// If no target...
	if (mCurrentTarget == "")
	{
		mpGLRect->setAlpha(0.0f);
		return;
	}

	// Get our target.
	GameObject* pTargetGO = GlobalManager::getGameObjectManager()->getGameObject(mCurrentTarget);
	if (pTargetGO == nullptr)
	{
		mpGLRect->setAlpha(0.0f);
		return;
	}

	// Get its transform
	Transform* pTargetTransf = static_cast<Transform*>(pTargetGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	if (pTargetTransf == nullptr)
	{
		// This really shouldn't ever happen... But just in case.
		mpGLRect->setAlpha(0.0f);
		return;
	}

	// We have our target. Track it.
	mpTransform->setPosition(pTargetTransf->getPosition());
	mpGLRect->setAlpha(mLockedAlpha);

}

void TargetLock::getNextTarget()
{

}

void TargetLock::handleEvent(Event* pEvent)
{
	// If our target is destroyed, revert to no target. 
	if (pEvent->mType == EventType::GAMEOBJECT_DESTROYED)
	{
		GameObjectDestroyedEvent* pGODestroyEvent = static_cast<GameObjectDestroyedEvent*>(pEvent);

		if (pGODestroyEvent->mDestroyedGOName == this->mCurrentTarget)
		{
			this->mCurrentTarget = "";
		}
	}
}


void TargetLock::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Alpha") && inputObj["Alpha"].IsNumber())
		this->mLockedAlpha = inputObj["Alpha"].GetFloat();
}