/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Timer.cpp
	Purpose				Implementations for timer.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/26/2020

- End Header --------------------------------------------------------*/

#include "Timer.h"
#include "../Managers/GlobalManager.h"

Timer::Timer() : Component(ComponentTypes::TYPE_TIMER)
{
}

Timer::~Timer()
{

}


void Timer::Initialize() {}

void Timer::Update()
{
	// Get frametime.

	// Update all timer events.

	//GlobalManager::getEventManager()->broadcastEventToSubscribers();
}


void Timer::handleEvent(Event* pEvent)
{
}


void Timer::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}