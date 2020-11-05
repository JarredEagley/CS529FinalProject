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
	glm::vec3 offset;
	float clipNear;
	float clipFar;
	float left;
	float right;
	float top;
	float bottom;
private:
	void buildTransform();
private:
	glm::mat4 projMatrix;
	glm::mat4 offsetTransMatrix;
	GameObject* mpParentGO; // May eventually be better placed in GameObject...

};

