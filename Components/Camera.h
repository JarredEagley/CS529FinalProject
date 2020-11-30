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

	glm::mat4 getProjMatrix() { return cameraProjection; }; // Gets the perspective transformation matrix. Currently ortho.
	glm::mat4 getTransMatrix() { return cameraTransform; }

	glm::vec3 getPosition(); // Applies transform component too.
	//glm::vec3 getForward();

	void handleEvent(Event* pEvent);

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	glm::vec3 offset = glm::vec3(0,0,0);
private:
	void buildTransform();
private:
	float mCameraAngle = 0.15f;
	Transform* mpTransform = nullptr;
	glm::mat4 cameraTransform = glm::mat4(1.0f);
	glm::mat4 cameraProjection = glm::mat4(1.0f);
	glm::mat4 cameraOffset = glm::mat4(1.0f);
	//GameObject* mpParentGO = nullptr; // May eventually be better placed in GameObject...

};

