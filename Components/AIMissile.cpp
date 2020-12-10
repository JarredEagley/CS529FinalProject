/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIMissile.h
	Purpose				AI Behavior for missiles. Will work practically
						identical to my missiles in Asteroids. These will
						be a decently effective but very finite long-range
						attack.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#include "AIMissile.h"

AIMissile::AIMissile() : Component(ComponentTypes::TYPE_UNDEFINED),
mActivateTimer(1000.0f), mTargetName(""), mWarheadIntensity(1000.0f), mDetonateDistance(100.0f)
{
}

AIMissile::~AIMissile()
{
}


void AIMissile::Initialize()
{
}

void AIMissile::Update()
{
	if (mpShipData == nullptr)
	{
		mpShipData = static_cast<ShipData*>(mpOwner->GetComponent(ComponentTypes::TYPE_SHIPDATA));
		return;
	}

	if (mActivateTimer < 0.0f)
	{
		// Do seeking logic.



	}
	else
		mActivateTimer -= GlobalManager::getPhysicsManager()->getGameTime();
}

void AIMissile::handleEvent(Event* pEvent)
{
}


void AIMissile::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Detonation Distance") && inputObj["Detonation Distance"].IsNumber())
	{
		this->mDetonateDistance = inputObj["Detonation Distance"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Missile component did not find a detonation distance parameter. Default used." << std::endl;


	if (inputObj.HasMember("Warhead Intensity") && inputObj["Warhead Intensity"].IsNumber())
	{
		this->mDetonateDistance = inputObj["Warhead Intensity"].GetFloat();
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Missile component did not find a warhead intensity parameter. Default used." << std::endl;
}