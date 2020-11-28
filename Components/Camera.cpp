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
	//pTransform->setZ(zoom);
	
	// Will be moved to a camera controller component.
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	if (GlobalManager::getInputManager()->getWheelY() != 0)
	{
		//zoom -= GlobalManager::getInputManager()->getWheelY() * zoom * 0.4f;
		GlobalManager::getGraphicsManager()->incrementZoomLevel(-GlobalManager::getInputManager()->getWheelY() * pGM->getZoomLevel() * 0.4f );
	}

	//std::cout << "DEBUG - offset = " << offset.x << ", " << offset.y << ", " << offset.z << std::endl;

	// Handle offset.
	int mX, mY;
	GlobalManager::getInputManager()->getMousePosition(mX, mY);
	int const winH = GlobalManager::getGraphicsManager()->windowHeight;
	int const winW = GlobalManager::getGraphicsManager()->windowWidth;
	this->offset.x = (mX-(winW/2)) * (pGM->getZoomLevel()/500.0f);
	this->offset.y = -(mY-(winH/2)) * (pGM->getZoomLevel()/500.0f);

	// Stuff to only do if not parented.
	if (this->mpOwner->getParent() == nullptr)
	{
		this->offset.x -= 10.0f; // Correct for weird offset. A bit hacky, but it works.

		// Build perspective transformation... // Transform now built on event call.
		buildTransform();
	}

}

void Camera::buildTransform()
{
	cameraProjection = glm::mat4(1.0f);
	//cameraProjection = glm::ortho(this->left, this->right, this->bottom, this->top, this->clipNear, this->clipFar);

	// Perspective
	cameraProjection = glm::perspective(45.0f, 1.0f, 0.1f, 1000000.0f); // TO-DO: Move these params back into somewhere more generalized.

	// Build transform.
	cameraTransform = (glm::mat4(1.0f))/(pTransform->getTransformationMatrix());
	cameraTransform = (pTransform->getTransformationMatrix());
	// Offset Translation.
	cameraTransform = glm::translate(cameraTransform, offset); 
	
	// Rotate.
	cameraTransform = glm::rotate(cameraTransform, 0.15f, glm::vec3(1, 0, 0));
	
	// Apply Zoom translation
	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f,0.0f, GlobalManager::getGraphicsManager()->getZoomLevel() ));

	cameraTransform = glm::inverse(cameraTransform);

	CameraTransformUpdatedEvent* pNewEvent = new CameraTransformUpdatedEvent();
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewEvent);
}


void Camera::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::TRANSFORM_UPDATED)
	{
		TransformUpdatedEvent* pTransformEvent = static_cast<TransformUpdatedEvent*>(pEvent);
		Transform* pParentTransform = pTransformEvent->mpTransform;

		// Check if this belongs to owner's parent.
		if (this->mpOwner->getParent() == pParentTransform->mpOwner)
		{
			// Apply translation.
			this->buildTransform();
		}
	}
}


void Camera::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// For now, I'll just auto-bind.
	GlobalManager::getGraphicsManager()->setCurrentCameraGO(this->mpOwner);

	pTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
}