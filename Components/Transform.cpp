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
	//mPositionX = mPositionY = 0.0f;
	mPosition = glm::vec4(0, 0, 0, 1);
	mRotation = 0;
	mScale = glm::vec3(1, 1, 1);
	mTransMatrix = glm::mat4(); // Identity.
}

Transform::~Transform()
{}

void Transform::Update()
{
	buildTransformationMatrix();
} 

void Transform::buildTransformationMatrix()
{
	// Build the transformations.
	glm::mat4 rot = glm::rotate( glm::mat4(),mRotation, glm::vec3(0,0,1) );
	glm::mat4 trans = glm::translate( glm::mat4(), glm::vec3(mPosition) );
	glm::mat4 scale = glm::scale( glm::mat4(), mScale );
	// Build the transformation matrix.
	mTransMatrix = glm::mat4() * trans * rot * scale;
}


void Transform::setX(float x)
{
	mPosition.x = x;
}

void Transform::setY( float y)
{
	mPosition.y = y;
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