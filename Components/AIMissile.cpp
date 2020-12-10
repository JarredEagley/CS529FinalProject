/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIMissile.h
	Purpose				AI Behavior for missiles. Will work practically
						identical to my missiles in Asteroids. These will
						be a decently effective but very finite long-range
						attack.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#include "AIMissile.h"

AIMissile::AIMissile() : Component(ComponentTypes::TYPE_UNDEFINED)
{
}

AIMissile::~AIMissile()
{
}


void AIMissile::Initialize()
{
}

void AIMissile::Update()
{
}

void AIMissile::handleEvent(Event* pEvent)
{
}


void AIMissile::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{



}