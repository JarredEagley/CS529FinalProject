#include "Camera.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

//#include "SDL.h" // FOR INPUT, WILL BE MOVED ELSEWHERE.

Camera::Camera() : Component(ComponentTypes::TYPE_CAMERA),
mCameraTransform(glm::mat4(1.0f)), mCameraProjection(glm::mat4(1.0f))
{
}

Camera::~Camera()
{
}


void Camera::Initialize() {}

void Camera::Update()
{
	if (mpTransform == nullptr)
	{
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

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
	int const winH = GlobalManager::getGraphicsManager()->mWindowHeight;
	int const winW = GlobalManager::getGraphicsManager()->mWindowWidth;
	
	this->offset.x = (mX-(winW/2)) * (pGM->getZoomLevel()/ (float)winW);
	this->offset.y = -(mY-(winH/2)) * (pGM->getZoomLevel()/ (float)winH);

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
	mCameraProjection = glm::mat4(1.0f);
	//cameraProjection = glm::ortho(this->left, this->right, this->bottom, this->top, this->clipNear, this->clipFar);

	// Perspective
	mCameraProjection = glm::perspective(45.0f, 1.0f, 0.1f, 1000000.0f); // TO-DO: Move these params back into somewhere more generalized.

	// Build transform.
	//cameraTransform = (glm::mat4(1.0f))/(mpTransform->getTransformationMatrix());
	mCameraTransform = (mpTransform->getTransformationMatrix());

	// Offset Translation.
	//cameraTransform = glm::translate(cameraTransform, offset); 
	
	// Rotate.
	mCameraTransform = glm::rotate(mCameraTransform, mCameraAngle, glm::vec3(1, 0, 0));
	
	// Apply Zoom translation
	mCameraTransform = glm::translate(mCameraTransform, glm::vec3(0.0f,0.0f, GlobalManager::getGraphicsManager()->getZoomLevel() ));

	mCameraTransform = glm::inverse(mCameraTransform);

	CameraTransformUpdatedEvent* pNewEvent = new CameraTransformUpdatedEvent();
	pNewEvent->mProjectionMatrix = this->mCameraProjection;
	pNewEvent->mViewMatrix = this->mCameraTransform;
	GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewEvent);
}

float Camera::getHeight()
{
	float zoom = GlobalManager::getGraphicsManager()->getZoomLevel();
	float height = sin(mCameraAngle) * zoom;

	return height;
}

/*
glm::vec3 Camera::getForward()
{
	 return glm::rotate(glm::mat4(1.0f), mCameraAngle, glm::vec3(1, 0, 0)) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
*/


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

	mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
}