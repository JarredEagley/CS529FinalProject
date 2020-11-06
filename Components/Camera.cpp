#include "Camera.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

#include "SDL.h" // FOR INPUT, WILL BE MOVED ELSEWHERE.

Camera::Camera() : Component(ComponentTypes::TYPE_CAMERA)
{
	glm::vec3 offset = glm::vec3(0.0f,0.0f,0.0f);
	
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

	// Just for fun, remove me in the future.
	if (GlobalManager::getInputManager()->IsKeyPressed(SDL_SCANCODE_S))
	{
		top = top + 10;
		bottom = bottom - 10;
		left = left - 10;
		right = right + 10;
	}
	if (GlobalManager::getInputManager()->IsKeyPressed(SDL_SCANCODE_W))
	{
		top = top - 10;
		bottom = bottom + 10;
		left = left + 10;
		right = right - 10;
	}
	if (GlobalManager::getInputManager()->IsKeyPressed(SDL_SCANCODE_A))
	{
		left = left + 10;
		right = right + 10;
	}
	if (GlobalManager::getInputManager()->IsKeyPressed(SDL_SCANCODE_D))
	{
		left = left - 10;
		right = right - 10;
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
	projMatrix =  glm::ortho(this->left, this->right, this->bottom, this->top, this->clipNear, this->clipFar);
	projMatrix = projMatrix * glm::rotate(glm::mat4(1.0f), glm::degrees(80.0f), glm::vec3(1,0,0));
	offsetTransMatrix = glm::translate(glm::mat4(1.0f), this->offset);
}

void Camera::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	std::cout << "DEBUG - Deserializing camera..." << std::endl;
	// For now, I'll just auto-bind.
	GlobalManager::getGraphicsManager()->setCurrentCamera(this->mpOwner);

	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	pTrans->setZ(5.0);
}