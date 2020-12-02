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
	float mCameraAngle = 0.0f;
	Transform* mpTransform = nullptr;
	glm::mat4 mCameraTransform;
	glm::mat4 mCameraProjection;
	//glm::mat4 cameraOffset;
	//GameObject* mpParentGO = nullptr; // May eventually be better placed in GameObject...

};

