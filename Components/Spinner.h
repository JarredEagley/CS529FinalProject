/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Spinner.h
	Purpose:			Given a spinrate, updates corrisponding transform
						component's angle to maintain a constant spin.
						Basically just used to add some visual spice.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/12/2020

- End Header --------------------------------------------------------*/
#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "Transform.h"
#include "../GameObject.h"

class Spinner : public Component
{
public:
	Spinner();
	~Spinner();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent) {};

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	float mSpinRate = 0.0f;
private:
	// No private methods.
private:
	Transform* mpTransform;
};

