/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Transform.cpp
	Purpose				Transform component tracks a GameObject's position
						in the world.
						May eventually track rotation angle as well.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "Transform.h"
#include "ComponentTypes.h"
#include <iostream>
#include "rapidjson/rapidjson.h"

Transform::Transform() : Component(ComponentTypes::TYPE_TRANSFORM) // Call the constructor of the base class with the correct type.
{
	mPositionX = mPositionY = 0.0f;
}

Transform::~Transform()
{}

void Transform::Update()
{} 

void Transform::setX(float x)
{
	mPositionX = x;
}

void Transform::setY( float y)
{
	mPositionY = y;
}

void Transform::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check for transform.
	if (!inputMemberIt->value.IsArray() 
		|| !inputMemberIt->value.GetArray()[0].IsNumber()
		|| !inputMemberIt->value.GetArray()[1].IsNumber() )
	{
		std::cerr << "Warning: Transform component failed to deserialize. Value was not an array of floats." << std::endl;
		return;
	}

	// Transform stored as an array of floats.
	//std::cout << "DEBUG: Transform is " << inputMemberIt->value.GetArray()[0].GetFloat() << ", " << inputMemberIt->value.GetArray()[1].GetFloat() << "\n";
	setX(inputMemberIt->value.GetArray()[0].GetFloat());
	setY(inputMemberIt->value.GetArray()[1].GetFloat());
}