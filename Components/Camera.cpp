#include "Camera.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

#include "SDL.h" // FOR INPUT, WILL BE MOVED ELSEWHERE.

Camera::Camera() : Component(ComponentTypes::TYPE_CAMERA)
{
	glm::vec3 offset = glm::vec3(0.0f,0.0f,0.0f);
	float clipNear = 0.1f;
	float clipFar = 10.0f;
	float left = 5.0f;
	float right = 5.0f;
	float top = 5.0f;
	float bottom = 5.0f;
	projMatrix = glm::mat4(1.0f);
	offsetTransMatrix = glm::mat4(1.0f);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// Update this game object's transformation based on parent if one exists...
	if (this->mpParentGO != nullptr)
	{
		Transform* pOwnerTransform = static_cast<Transform*>(this->mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		Transform* pParentTransform = static_cast<Transform*>(this->mpParentGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		pOwnerTransform->setPosition(pParentTransform->getPosition()); // Just set position. No need to alter camera angle.
	}


	if (GlobalManager::getInputManager()->IsKeyPressed(SDL_SCANCODE_W))
	{
		std::cout << "DEBUG - Trying to zoom out camera" << std::endl;
		top = top + 10;
		bottom = bottom + 10;
		left = left + 10;
		right = right + 10;
	}

	// Handle offset (todo)

	// Build perspective transformation...
	buildTransform();
}

void Camera::assignParent(GameObject* pGO)
{
	this->mpParentGO = pGO;
}

void Camera::buildTransform()
{
	projMatrix = glm::ortho(this->left, this->right, this->bottom, this->top, this->clipNear, this->clipFar);
	offsetTransMatrix = glm::translate(glm::mat4(0.1f), this->offset);
}

void Camera::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	std::cout << "DEBUG - Deserializing camera..." << std::endl;
	// For now, I'll just auto-bind.
	GlobalManager::getGraphicsManager()->setCurrentCamera(this->mpOwner);
}