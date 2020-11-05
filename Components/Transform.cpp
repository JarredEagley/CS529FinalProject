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
	mPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mRotation = 0;
	mScale = glm::vec3(1, 1, 1);
	//mTransMatrix = glm::mat4(1.0f); // Identity.
	buildTransformationMatrix();
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
	mTransMatrix = glm::mat4(1.0f);
	mTransMatrix = glm::translate(mTransMatrix, glm::vec3(mPosition));
	mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mRotation), glm::vec3(0,0,1));
	mTransMatrix = glm::scale(mTransMatrix, mScale);

	//std::cout << "built transformation matrix. (" << mPosition.x << ", " << mPosition.y << ")" << std::endl;
}

void Transform::setPosition(glm::vec3 pos)
{
	this->mPosition = glm::vec4(pos, 1.0f);
}

void Transform::setPosition(glm::vec4 pos)
{
	this->mPosition = pos;
}

void Transform::setRotation(float rot)
{
	this->mRotation = rot;
}

void Transform::setX(float x)
{
	mPosition.x = x;
}

void Transform::setY( float y)
{
	mPosition.y = y;
}

void Transform::setZ(float z)
{
	mPosition.z = z;
}

glm::mat4 Transform::getTransformationMatrix() 
{ 
	// Do I need to nullcheck here? 
	return this->mTransMatrix; 
};


void Transform::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// TO-DO: Redo
	/*
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
	*/
}