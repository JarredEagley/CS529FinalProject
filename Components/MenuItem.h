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

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

// Unfortunatly, forward declaration wont work here. We'll need the full include for the implementation.
#include "Transform.h"
#include "GLRect.h"

class MenuItem : public Component
{
public:
	MenuItem();
	~MenuItem();

	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);
	
	void setUniformData(ShaderProgram* pProgram);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private methods.
private:
	Transform* mpTransform;
	GLRect* mpGLRect;
};

