/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Transform.h
	Purpose				Transform component tracks a GameObject's position
						in the world.
						May eventually track rotation angle as well.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "rapidjson/document.h"

//class GameObject;

class Transform : public Component 
{
public:
	Transform();
	~Transform();
	void Update();

	void setX(float x);
	void setY(float y);

	float getX() { return mPositionX; };
	float getY() { return mPositionY; };

	void incrementX(float delta) { mPositionX += delta; }; // TO-DO: Maybe this should be in cpp...
	void incrementY(float delta) { mPositionY += delta; };
	
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	float mPositionX, mPositionY;
};