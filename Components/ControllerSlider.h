/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ControllerSlider.h
	Purpose:			Header file for a 'slider' controller component.
						This is extremely simple and just uses the arrow
						keys and frametime to update position.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/19/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "rapidjson/document.h"

class ControllerSlider : public Component
{
public:
	ControllerSlider();
	ControllerSlider(float speed);
	~ControllerSlider();
	
	void Update();
	void setSpeed(float speed);
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	float mspeed;
};

