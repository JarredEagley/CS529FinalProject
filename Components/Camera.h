#pragma once

#include "Component.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	void Update();

	void assignParent(GameObject* pGO); // Assigns parent game object for transformations. This functionality may eventually be better placed in GameObject itself.

	glm::mat4 getProjMatrix() { return projMatrix; }; // Gets the perspective transformation matrix. Currently ortho.

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	glm::vec3 offset = glm::vec3(0,0,0);
	float clipNear = -100.0f;
	float clipFar = 100.0f;
	float left = -10.0f;
	float right = 10.0f;
	float top = 10.0f;
	float bottom = -10.0f;
private:
	void buildTransform();
private:
	glm::mat4 projMatrix = glm::mat4(1.0f);
	glm::mat4 offsetTransMatrix = glm::mat4(1.0f);
	GameObject* mpParentGO = nullptr; // May eventually be better placed in GameObject...

	float scale = 10; // Will go in camera controller
};

