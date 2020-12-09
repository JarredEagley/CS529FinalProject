/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			MenuItem.h
	Purpose				Menu components will be used to build the majority
						of menu structures in the game. Clicking on them
						broadcasts an event for the specific item.
						Each menu item will be a game object.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/8/2020

- End Header --------------------------------------------------------*/

#include "MenuItem.h"


MenuItem::MenuItem() : Component(ComponentTypes::TYPE_MENUITEM)
{

}

MenuItem::~MenuItem()
{

}

void MenuItem::Initialize()
{

}


void MenuItem::Update()
{

}

void MenuItem::handleEvent(Event* pEvent)
{

}

void MenuItem::setUniformData(ShaderProgram* pProgram) {}

void MenuItem::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) {}