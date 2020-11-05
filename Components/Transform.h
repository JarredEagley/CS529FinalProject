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

#include "glm/glm.hpp" // Implement transformations!!!
#include "glm/gtc/matrix_transform.hpp"

class Transform : public Component 
{
public:
	Transform();
	~Transform();

	void Update();

	void setPosition(glm::vec3);
	void setPosition(glm::vec4);
	void setRotation(float);
	void setX(float x);
	void setY(float y);

	glm::vec3 getPosition() { return mPosition; };
	float getRotation() { return mRotation; };
	glm::mat4 getTransformationMatrix() ;

	void incrementX(float delta) { mPosition.x += delta; }; // TO-DO: Maybe this should be in cpp...
	void incrementY(float delta) { mPosition.y += delta; };
	void incrementRotation(float delta) { mRotation += delta; };
	
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	void Transform::buildTransformationMatrix();
private:
	glm::vec4 mPosition;
	float mRotation; // I'm not going to bother supporting 3D rotation.
	glm::vec3 mScale;
	glm::mat4 mTransMatrix;
};