/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Spinner.cpp
	Purpose:			Implementations for Spinner.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/12/2020

- End Header --------------------------------------------------------*/

#include "Spinner.h"
#include "../Managers/GlobalManager.h"

Spinner::Spinner() : Component(ComponentTypes::TYPE_SPINNER)
{
}

Spinner::~Spinner()
{
}


void Spinner::Initialize()
{
}

void Spinner::Update()
{
	if (mpTransform == nullptr)
	{
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}
	mpTransform->incrementRotation(mSpinRate * GlobalManager::getPhysicsManager()->getGameTime());
}

void Spinner::handleEvent(Event* pEvent)
{
}


void Spinner::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Spin Rate") && inputObj["Spin Rate"].IsNumber())
	 	this->mSpinRate = inputObj["Spin Rate"].GetFloat();
}