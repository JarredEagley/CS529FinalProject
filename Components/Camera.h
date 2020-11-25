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

	void Update();

	//void assignParent(GameObject* pGO); // Assigns parent game object for transformations. This functionality may eventually be better placed in GameObject itself.

	glm::mat4 getProjMatrix() { return cameraProjection; }; // Gets the perspective transformation matrix. Currently ortho.
	glm::mat4 getTransMatrix() { return cameraTransform; }

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	glm::vec3 offset = glm::vec3(0,0,0);
private:
	void buildTransform();
private:
	Transform* pTransform = nullptr;
	glm::mat4 cameraTransform = glm::mat4(1.0f);
	glm::mat4 cameraProjection = glm::mat4(1.0f);
	glm::mat4 cameraOffset = glm::mat4(1.0f);
	//GameObject* mpParentGO = nullptr; // May eventually be better placed in GameObject...

	float zoom = 10; // Will go in camera controller
};

