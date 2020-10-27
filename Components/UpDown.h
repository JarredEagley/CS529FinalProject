/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Transform.h
	Purpose				The UpDown component is a super basic 'AI' mainly
						just to show off data-driven design.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/22/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include <istream>
#include "rapidjson/document.h"

class UpDown : public Component
{
public:
	UpDown();
	~UpDown() {};

	void Update();
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	bool mGoingUp;
	int mTimer; 
	int mTimerLimit; // TO-DO: Probably need to make timer a component...
private:
	// No private methods.
private:
	// No private variables.
};

