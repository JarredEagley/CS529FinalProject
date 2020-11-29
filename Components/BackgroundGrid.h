/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			BackgroundGrid.h
	Purpose				Background grid which gives the player some frame 
						of reference while moving. Similar to the direction
						indicators, this fades in and out with zoom level,
						but is actually the other way around, becoming less
						noticable when zoomed in.
						Scales size, uv coords, changes alpha. Therefore
						will need to interface with
						transform and GLRect.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/27/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "GLRect.h"
#include "Transform.h"
#include "Camera.h"

class BackgroundGrid : public Component
{
public:
	BackgroundGrid();
	~BackgroundGrid();

	void Initialize();
	void Update();

	void transformEventUpdate(Event* pEvent);
	void scrollEventUpdate(Event* pEvent);

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	GLRect* mpGLRect;
	Transform* mpTransform;
	float mGridScale, mGridIntensity;
};

