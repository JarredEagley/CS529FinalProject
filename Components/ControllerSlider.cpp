/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ControllerSlider.h
	Purpose:			Implementation for the 'slider' type of controller.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/19/2020

- End Header --------------------------------------------------------*/

#include "ControllerSlider.h"
#include "../GameObject.h"
#include "Transform.h"
#include <iostream>

#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

#include "../../SDL2.0/include//SDL_scancode.h"

ControllerSlider::ControllerSlider() : Component(ComponentTypes::TYPE_CONTROLLERSLIDER), mspeed(1.0f)
{
	//mspeed = 1.0f; // Default speed. I think there's a more efficient way to do this default ctor.
}

ControllerSlider::ControllerSlider(float speed) : Component(ComponentTypes::TYPE_CONTROLLERSLIDER)
{
	mspeed = speed;
}

ControllerSlider::~ControllerSlider() {}

void ControllerSlider::Update()
{
	//Transform* pT = (Transform*)(mpOwner->GetComponent(TYPE_TRANSFORM));
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));

	if (nullptr == pT)
		return;

	InputManager* pIM = GlobalManager::getInputManager();
	FrameRateController* pFRC = GlobalManager::getFrameRateController();
	
	// If key press.
	if (pIM->IsKeyPressed(SDL_SCANCODE_W))
		pT->incrementY( (float)(mspeed * pFRC->getFrameTime()) );
	if (pIM->IsKeyPressed(SDL_SCANCODE_S))
		pT->incrementY(  -(float)(mspeed * pFRC->getFrameTime()) );
	if (pIM->IsKeyPressed(SDL_SCANCODE_A))
		pT->incrementX( -(float)(mspeed * pFRC->getFrameTime()) );
	if (pIM->IsKeyPressed(SDL_SCANCODE_D))
		pT->incrementX(  (float)(mspeed * pFRC->getFrameTime()) );

	if (pIM->IsKeyPressed(SDL_SCANCODE_E))
		pT->incrementRotation((float)( - 0.1f * pFRC->getFrameTime()));
	else if (pIM->IsKeyPressed(SDL_SCANCODE_Q))
		pT->incrementRotation((float)( 0.1f * pFRC->getFrameTime()));

}

void ControllerSlider::setSpeed(float speed)
{
	mspeed = speed;
}

void ControllerSlider::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Check to ensure value makes sense.
	if (!inputMemberIt->value.IsFloat())
	{
		std::cerr << "Warning: ControllerSlider did not recieve float value; deserialization failed." << std::endl;
		return;
	}
	// Speed Is stored as a float.
	setSpeed(inputMemberIt->value.GetFloat());
}