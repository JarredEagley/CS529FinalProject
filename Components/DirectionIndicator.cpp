/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			DirectionIndicator.cpp
	Purpose				Implementations for DirectionIndicator.
						Will interface with PhysicsBody and Transform.
						May eventually track rotation angle as well.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/25/2020

- End Header --------------------------------------------------------*/

#include "DirectionIndicator.h"
#include "ComponentTypes.h"

DirectionIndicator::DirectionIndicator() : Component(ComponentTypes::TYPE_DIRINDICATOR), mIndicatorScaleFactor(1.0)
{

}

DirectionIndicator::~DirectionIndicator()
{

}


void DirectionIndicator::Update() 
{
	// 
}


void DirectionIndicator::handleEvent(Event* pEvent)
{

}


void DirectionIndicator::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}