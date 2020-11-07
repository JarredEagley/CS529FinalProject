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

	this->right = (float)(GlobalManager::getGraphicsManager()->windowWidth) * scale/1000.0f;
	this->left = -(float)(GlobalManager::getGraphicsManager()->windowWidth) * scale/1000.0f;
	this->top = (float)(GlobalManager::getGraphicsManager()->windowHeight) * scale/1000.0f;
	this->bottom = -(float)(GlobalManager::getGraphicsManager()->windowHeight) * scale/1000.0f;

	// Will be moved to a camera controller component.
	if (GlobalManager::getInputManager()->getWheelY() != 0)
	{
		scale -= GlobalManager::getInputManager()->getWheelY() * scale * 0.4f;
	}

	// Handle offset.
	int mX, mY;
	GlobalManager::getInputManager()->getMousePosition(mX, mY);
	this->offset.x = ( -mX + (GlobalManager::getGraphicsManager()->windowWidth/2.0f) ) * scale / 1000.0f; // TO-DO: Right idea, will need scaling factor.
	this->offset.y = ( -mY + (GlobalManager::getGraphicsManager()->windowHeight/2.0f) ) * scale / 1000.0f;

		// Build perspective transformation...
	buildTransform();
}

void Camera::assignParent(GameObject* pGO)
{
	this->mpParentGO = pGO;
}

void Camera::buildTransform()
{
	// Get transform. 
	glm::mat4 camTransform = glm::mat4(1.0f);
	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	if (pTrans == nullptr)
		std::cout << "Warning: Camera component's GameObject did not have transform component." << std::endl;
	else
	{
		camTransform = pTrans->getTransformationMatrix();
	}
	projMatrix = glm::ortho(this->left, this->right, this->bottom, this->top, this->clipNear, this->clipFar);
	projMatrix = projMatrix * glm::rotate(glm::mat4(1.0f), glm::degrees(80.0f), glm::vec3(1,0,0)); // Building this into the proj matrix. A bit hacky, but it works.

	offsetTransMatrix = glm::translate(glm::mat4(1.0f), this->offset);
	projMatrix = projMatrix * offsetTransMatrix;
}

void Camera::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// For now, I'll just auto-bind.
	GlobalManager::getGraphicsManager()->setCurrentCamera(this->mpOwner);

	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	pTrans->setZ(5.0);
}