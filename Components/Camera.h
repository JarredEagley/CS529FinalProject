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
	float clipNear = 0;
	float clipFar = 0;
	float left = 0;
	float right = 0;
	float top = 0;
	float bottom = 0;
private:
	void buildTransform();
private:
	glm::mat4 projMatrix = glm::mat4(1.0f);
	glm::mat4 offsetTransMatrix = glm::mat4(1.0f);
	GameObject* mpParentGO = nullptr; // May eventually be better placed in GameObject...

};

