/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Camera.h
	Purpose				Encapsulates the behavior expected of a camera
						GameObject, including handling the perspective
						transformation, and view transformation.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/5/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update();

	glm::mat4 getProjMatrix() { return this->mCameraProjection; }; 
	glm::mat4 getTransMatrix() { return this->mCameraTransform; };

	float getHeight();
	float getAngle() { return mCameraAngle; };

	//glm::vec3 getForward();

	void handleEvent(Event* pEvent);

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	glm::vec3 offset = glm::vec3(0.0f);
private:
	void buildTransform();
private:
	float mCameraAngle = 0.15f;
	Transform* mpTransform = nullptr;
	glm::mat4 mCameraTransform;
	glm::mat4 mCameraProjection;
	//glm::mat4 cameraOffset;
	//GameObject* mpParentGO = nullptr; // May eventually be better placed in GameObject...

};

