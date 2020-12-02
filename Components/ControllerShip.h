/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ControllerShip.h
	Purpose				Takes player input and relays it to a ShipData
						component.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/7/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "rapidjson/document.h"
#include "ShipData.h"

class ControllerShip : public Component
{
public:
	ControllerShip();
	//ControllerShip(float speed);
	~ControllerShip();
	
	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	float mAngularDamping; // How agressively ship tries to stop rotating with no input.
	glm::vec3 mPlayerAimPoint = glm::vec3(0.0f);

private:
	// No private methods.
private:
	ShipData* mpShipData;
	

};

