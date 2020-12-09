/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Transform.cpp
	Purpose				Transform component tracks a GameObject's position
						in the world.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "Transform.h"
#include "ComponentTypes.h"
#include <iostream>
#include "rapidjson/rapidjson.h"
#include "../Managers/GlobalManager.h"
//#include "../GameObject.h"

Transform::Transform() : Component(ComponentTypes::TYPE_TRANSFORM), mInheritRotation(false), // Call the constructor of the base class with the correct type.
mPosition(glm::vec4(0.0f)), mRotation(0.0f), mScale(glm::vec3(1.0f)), mTransMatrix(glm::mat4(1.0f))
{
	initTransformationMatrix();
}

Transform::~Transform()
{}


void DEBUG_PrintMatrix(glm::mat4 input)
{
	std::cout << "Debug print for matrix:" << std::endl;
	for (int row = 0; row < 4; ++row)
	{
		std::cout << "[";
		for (int col = 0; col < 4; ++col)
		{
			std::cout << " " << input[col][row];
		}
		std::cout << " ]\n";
	}
	std::cout << "-------------------------" << std::endl;
}

void Transform::Initialize() {}

void Transform::Update()
{
	buildTransformationMatrix();
	
	// Send transform update to children.
	if (this->mpOwner->hasChildren())
	{
		TransformUpdatedEvent* pTransformEvent = new TransformUpdatedEvent(this);
		GlobalManager::getEventManager()->broadcastEventToSubscribers(pTransformEvent);
	}
} 

// Called on initialization when mpowner is still nullptr.
void Transform::initTransformationMatrix()
{
	mTransMatrix = glm::mat4(1.0f);
	mTransMatrix = glm::translate(mTransMatrix, glm::vec3(mPosition));
	mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mRotation), glm::vec3(0, 0, 1));
	mTransMatrix = glm::scale(mTransMatrix, mScale);
}

// Build without parent.
void Transform::buildTransformationMatrix()
{
	// Only proceed if this object isn't parented.
	if (mpOwner->getParent() != nullptr)
		return;

	//std::cout << "DEBUG - Transform build matrix for " << mpOwner->mName << std::endl;
	mTransMatrix = glm::mat4(1.0f);

	// Build the transformations.
	mTransMatrix = glm::translate(mTransMatrix, glm::vec3(mPosition));
	mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mRotation), glm::vec3(0,0,1));
	mTransMatrix = glm::scale(mTransMatrix, mScale);


	// Most of the time these will just be zeros. A bit inefficient, but that's okay.
	/*
	mTransMatrix = glm::translate(mTransMatrix, glm::vec3(mPrevParentPos, 0.0f));
	mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mPrevParentRot), glm::vec3(0, 0, 1));
	mPrevParentPos = glm::vec2(0.0f);
	mPrevParentRot = 0.0f;
	*/

	mTransMatrix *= mPrevParentedTransform;

	//std::cout << mpOwner->mName << " built transformation matrix. (" << mPosition.x << ", " << mPosition.y << ")" << std::endl;
}

// Build with parent.
void Transform::buildTransformationMatrix(Transform* pParentTransform)
{
	//std::cout << mpOwner->mName << "'s parent is " << mpOwner->getParent()->mName << std::endl;

	mTransMatrix = glm::mat4(1.0f);
	
	// Try to apply parent transform.
	if (pParentTransform != nullptr)
	{
		mTransMatrix = glm::translate(mTransMatrix, pParentTransform->getPosition());
		if (mInheritRotation)
		{
			mTransMatrix = glm::rotate(mTransMatrix, glm::radians(pParentTransform->getRotation()), glm::vec3(0,0,1));
		}
	}

	// Build the transformations.
	mTransMatrix = glm::translate(mTransMatrix, glm::vec3(mPosition));
	mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mRotation), glm::vec3(0, 0, 1));
	mTransMatrix = glm::scale(mTransMatrix, mScale);

	// Doesnt do anything unless this GO had a parent which died.
	mPrevParentedTransform = mTransMatrix;

	//std::cout << "built transformation matrix. (" << mPosition.x << ", " << mPosition.y << ")" << std::endl;
}

void Transform::setPosition(glm::vec2 pos)
{
	this->mPosition.x = pos.x; this->mPosition.y = pos.y;
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


void Transform::setScale(float scale)
{
	mScale = glm::vec3(scale);
}

void Transform::setScale(glm::vec2 scale)
{
	mScale = glm::vec3(scale, 1.0f);
}


glm::mat4 Transform::getTransformationMatrix() 
{ 
	return mTransMatrix; 
};


void Transform::handleEvent(Event* pEvent)
{
	// Parent transform update
	if (pEvent->mType == EventType::TRANSFORM_UPDATED)
	{
		TransformUpdatedEvent* pTransformEvent = static_cast<TransformUpdatedEvent*>(pEvent);
		Transform* pParentTransform = pTransformEvent->mpTransform;

		// Check if this belongs to owner's parent.
		if (this->mpOwner->getParent() == pParentTransform->mpOwner)
		{
			// Apply translation.
			buildTransformationMatrix(pParentTransform);
		}
	}

	// Position set ordered by event
	if (pEvent->mType == EventType::TRANSFORM_SETPOSITION)
	{
		SetTransformPositionEvent* pPositionEvent = static_cast<SetTransformPositionEvent*>(pEvent);
		this->setPosition(pPositionEvent->mPos);
	}


}


void Transform::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check for transform.
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Error: Transform component failed to deserialize. Value was not an object." << std::endl;
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
			setY( transObj["Translation"].GetArray()[1].GetFloat() );
			if (transObj["Translation"].GetArray().Size() > 2)
			setX( transObj["Translation"].GetArray()[2].GetFloat() ); // Optional Z.
		}
		else
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Transform component's 'Translation' member was incorrectly formatted." << std::endl;
	}
	else
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
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
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Transform component's 'Rotation' member was incorrectly formatted." << std::endl;
	}
	else
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
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
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Transform component's 'Scale' member was incorrectly formatted." << std::endl;
	}
	else
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Deserialized Transform component did not contain a Scale member." << std::endl;
	}

	// Check inherit rotation.
	if (transObj.HasMember("Inherit Rotation"))
	{
		// Doing a naiive impementation for this. Additional checks would be nice; but not necessary.
		mInheritRotation = true;
	}

}