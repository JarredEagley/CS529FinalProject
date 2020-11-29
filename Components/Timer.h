/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Timer.h
	Purpose				A timer component which other components can ask
						to respond to in a given amount of time.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/26/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

// When another component requests something be timed, timer will wait the given
// amount of time then broadcast a timer event to any gameobject subscribed to timerEvents.?
/*
class TimerRequest
{
public:
	TimerRequest(float time) : mStartTime(time), mTime(time) {};
	~TimerRequest() {};
	float mStartTime;
	float mTime;
};
*/

class Timer : public Component
{
public:
	Timer();
	~Timer();

	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	// No private variables.
};

