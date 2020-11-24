#include "Camera.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

#include "SDL.h" // FOR INPUT, WILL BE MOVED ELSEWHERE.

Camera::Camera() : Component(ComponentTypes::TYPE_CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// Update this game object's transformation based on parent if one exists...
	/*
	if (this->mpParentGO != nullptr)
	{
		Transform* pOwnerTransform = static_cast<Transform*>(this->mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		Transform* pParentTransform = static_cast<Transform*>(this->mpParentGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		pOwnerTransform->setPosition(pParentTransform->getPosition()); // Just set position. No need to alter camera angle.
	}
	*/

	this->right =   (float)(GlobalManager::getGraphicsManager()->windowWidth) * scale/1000.0f;
	this->left =   -(float)(GlobalManager::getGraphicsManager()->windowWidth) * scale/1000.0f;
	this->top =     (float)(GlobalManager::getGraphicsManager()->windowHeight) * scale/1000.0f;
	this->bottom = -(float)(GlobalManager::getGraphicsManager()->windowHeight) * scale/1000.0f;

	pTransform->setZ(scale);
	

	// Will be moved to a camera controller component.
	if (GlobalManager::getInputManager()->getWheelY() != 0)
	{
		scale -= GlobalManager::getInputManager()->getWheelY() * scale * 0.4f;
	}

	//std::cout << "DEBUG - offset = " << offset.x << ", " << offset.y << ", " << offset.z << std::endl;

	// Handle offset.
	int mX, mY;
	GlobalManager::getInputManager()->getMousePosition(mX, mY);
	int const winH = GlobalManager::getGraphicsManager()->windowHeight;
	int const winW = GlobalManager::getGraphicsManager()->windowWidth;
	this->offset.x = -(mX-(winW/2)) * (scale/500.0f) ; // (these are inverted to get the correct behavior.
	this->offset.y = (mY-(winH/2)) * (scale/500.0f)  ;

	// Build perspective transformation...
	buildTransform();
}
/*
void Camera::assignParent(GameObject* pGO)
{
	this->mpParentGO = pGO;
}
*/

void Camera::buildTransform()
{
	cameraProjection = glm::mat4(1.0f);
	//cameraProjection = glm::ortho(this->left, this->right, this->bottom, this->top, this->clipNear, this->clipFar);

	cameraProjection = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);

	cameraTransform = (glm::mat4(1.0f))/(pTransform->getTransformationMatrix());
	//cameraTransform = (pTransform->getTransformationMatrix());
	

	//cameraTransform = glm::translate(cameraTransform, glm::vec3(0,0,0));
	//cameraTransform = glm::rotate(cameraTransform, glm::degrees(45.0f), glm::vec3(1, 0, 0));

	//projMatrix = glm::translate(projMatrix, camTransform);
	
	//cameraProjection = glm::translate(cameraProjection, offset);
	//cameraRotation = glm::rotate(glm::mat4(1.0f), glm::degrees(-80.0f), glm::vec3(1,0,0)); // Building this into the proj matrix. A bit hacky, but it works.
}

void Camera::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// For now, I'll just auto-bind.
	GlobalManager::getGraphicsManager()->setCurrentCamera(this->mpOwner);

	pTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
	pTransform->setZ(5.0);
}