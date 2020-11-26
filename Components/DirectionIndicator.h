/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			DirectionIndicator.h
	Purpose				A component for the direction indicators which appear
						over ships and stations as the player zooms out.
						This may eventually be used for prograde-vector as well
						as ship-forward.
						Will interface with PhysicsBody and Transform.
						May eventually track rotation angle as well.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/25/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "GLRect.h"
#include "Transform.h"

class DirectionIndicator : public Component
{
public:
	DirectionIndicator();
	~DirectionIndicator();

	void Update();

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	float mIndicatorSizeFactor = 2000.0f;
	float mIndicatorAlphaFactor = 10.0f;
private:
	// No private methods.
private:
	GLRect* mpGLRect;
	Transform* mpTransform;
};

