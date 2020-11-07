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
#include "../GameObject.h"

Transform::Transform() : Component(ComponentTypes::TYPE_TRANSFORM) // Call the constructor of the base class with the correct type.
{
	//mPositionX = mPositionY = 0.0f;
	mPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mRotation = 0;
	mScale = glm::vec3(1, 1, 1);
	//mTransMatrix = glm::mat4(1.0f); // Identity.
	buildTransformationMatrix();
	pParentTransform = nullptr;
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

void Transform::setParent(GameObject* pGO)
{
	pParentTransform = static_cast<Transform*>(pGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));
}

void Transform::setParent(Transform* pTran)
{
	pParentTransform = pTran;
}



glm::mat4 Transform::getTransformationMatrix() 
{ 
	// Do I need to nullcheck here? 
	return this->mTransMatrix; 
};


void Transform::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check for transform.
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Warning: Transform component failed to deserialize. Value was not an object." << std::endl;
		return;
	}

	// Contains the values for the transform. 
	rapidjson::GenericObject<true, rapidjson::Value> transObj = inputMemberIt->value.GetObject();

	// Check if it has translation.
	if (transObj.HasMember("Translation"))
	{
		if (transObj["Translation"].IsArray() && transObj["Translation"].GetArray().Size() >= 2 && transObj["Translation"].GetArray()[0].IsNumber())
		{
			setX( transObj["Translation"].GetArray()[0].GetFloat() );
			setX( transObj["Translation"].GetArray()[1].GetFloat() );
			if (transObj["Translation"].GetArray().Size() > 2)
				setX( transObj["Translation"].GetArray()[2].GetFloat() ); // Optional Z.
		}
		else
			std::cout << "Warning: Deserialized Transform component's 'Translation' member was incorrectly formatted." << std::endl;
	}
	else
	{
		std::cout << "Warning: Deserialized Transform component did not contain a Translation member." << std::endl;
	}

	// Check if it has rotation.
	if (transObj.HasMember("Rotation"))
	{
		if (transObj["Rotation"].IsNumber())
		{
			setRotation(transObj["Rotation"].GetFloat());
		}
		else
			std::cout << "Warning: Deserialized Transform component's 'Rotation' member was incorrectly formatted." << std::endl;
	}
	else
	{
		std::cout << "Warning: Deserialized Transform component did not contain a Rotation member." << std::endl;
	}

	// Check if it has scale.
	if (transObj.HasMember("Scale"))
	{
		if (transObj["Scale"].IsArray() && transObj["Scale"].GetArray().Size() >= 2 && transObj["Scale"].GetArray()[0].IsNumber())
		{
			this->mScale = glm::vec3( // There may be a better way of doing this.
				transObj["Scale"].GetArray()[0].GetFloat(),
				transObj["Scale"].GetArray()[1].GetFloat(),
				1.0f
			);
		}
		else
			std::cout << "Warning: Deserialized Transform component's 'Scale' member was incorrectly formatted." << std::endl;
	}
	else
	{
		std::cout << "Warning: Deserialized Transform component did not contain a Scale member." << std::endl;
	}

	// Check if it has a parent GameObject.

}