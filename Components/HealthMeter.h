/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			HealthMeter.h
	Purpose				A component for the player's health indicator.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/6/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class GLRect;

class HealthMeter : public Component
{
public:
	HealthMeter();
	~HealthMeter();

	void Initialize();
	void Update();

	void setUniformData(ShaderProgram* pProgram);

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private methods.
private:
	glm::vec4 mSecondaryColor; // Color it fades toward as it depletes.
	float mHealth;
};

