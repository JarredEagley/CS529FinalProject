/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Transform.cpp
	Purpose				The UpDown component is a super basic 'AI' mainly
						just to show off data-driven design.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/22/2020

- End Header --------------------------------------------------------*/

#include "UpDown.h"
#include "ComponentTypes.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../Managers/GlobalManager.h"
#include "rapidjson/document.h"

#include <iostream>

UpDown::UpDown() : Component(ComponentTypes::TYPE_UPDOWN)
{
	mGoingUp = false;
	mTimer = 0; // ms
	mTimerLimit = 0;
}

void UpDown::Update()
{
	mTimer -= GlobalManager::getFrameRateController()->getFrameTime();
	
	if (mTimer <= 0)
	{
		mGoingUp = !mGoingUp;
		mTimer = mTimerLimit;
	}

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));

	if (nullptr != pT)
	{
		if (mGoingUp)
			pT->incrementY(0.2f * GlobalManager::getFrameRateController()->getFrameTime());
		else
			pT->incrementY(-0.2f * GlobalManager::getFrameRateController()->getFrameTime());
	}
}

void UpDown::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Check to make sure value makes sense for UpDown component.
	if (!inputMemberIt->value.IsArray()
		|| !inputMemberIt->value.GetArray()[0].IsInt()
		|| !inputMemberIt->value.GetArray()[1].IsInt())
	{
		std::cerr << "Warning: UpDown component failed to deserialize. Input was not an array of ints." << std::endl;
		return;
	}

	mGoingUp = (bool)inputMemberIt->value.GetArray()[0].GetInt();
	mTimerLimit = inputMemberIt->value.GetArray()[1].GetInt();
	
	mTimer = mTimerLimit;
}