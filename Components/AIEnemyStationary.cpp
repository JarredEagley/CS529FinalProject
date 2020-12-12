/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIEnemyStationary.cpp
	Purpose:			Implementations for AIEnemyStationary.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#include "AIEnemyStationary.h"
#include "../Managers/GlobalManager.h"


AIEnemyStationary::AIEnemyStationary() : Component(ComponentTypes::TYPE_AI_STATIONARY)
{
}

AIEnemyStationary::~AIEnemyStationary()
{
}


void AIEnemyStationary::Initialize()
{
}


void AIEnemyStationary::Update()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;

	if (mpAICore == nullptr)
	{
		mpAICore = static_cast<AIEnemyCore*>(mpOwner->GetComponent(ComponentTypes::TYPE_AI_CORE));
	}

	// --- behavior --- //

	if (mpAICore->isInGravity)
	{
		mpAICore->keepOrbit();
	}
	else
	{
		mpAICore->tryToStop();
	}
}


void AIEnemyStationary::handleEvent(Event* pEvent)
{
}


void AIEnemyStationary::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Nothing to deserialize.
}