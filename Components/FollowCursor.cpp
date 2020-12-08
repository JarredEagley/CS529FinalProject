/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FollowCursor.cpp
	Purpose				Implementations for FollowCursor.h.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/29/2020

- End Header --------------------------------------------------------*/

#include "FollowCursor.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

FollowCursor::FollowCursor() : Component(ComponentTypes::TYPE_FOLLOWCURSOR),
mpCameraComponent(nullptr), mpTransform(nullptr)
{
}

FollowCursor::~FollowCursor()
{}


void FollowCursor::Initialize()
{
	GlobalManager::getEventManager()->Subscribe(EventType::CAMERA_TRANSFORM_UPDATED, mpOwner);
}

void FollowCursor::Update()
{
}

void FollowCursor::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::CAMERA_TRANSFORM_UPDATED)
	{
		CameraTransformUpdatedEvent* pCameraTransformEvent = static_cast<CameraTransformUpdatedEvent*>(pEvent);

		EventManager* pEM = GlobalManager::getEventManager();
		InputManager* pIM = GlobalManager::getInputManager();
		GraphicsManager* pGM = GlobalManager::getGraphicsManager();

		// Need the camera for its projection matrix.
		if (mpCameraComponent == nullptr || mpTransform == nullptr || mpCameraTransformComponent == nullptr)
		{
			mpCameraComponent = static_cast<Camera*>(pGM->getCurrentCameraGO()->GetComponent(ComponentTypes::TYPE_CAMERA));
			mpCameraTransformComponent = static_cast<Transform*>(pGM->getCurrentCameraGO()->GetComponent(ComponentTypes::TYPE_TRANSFORM));
			mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
			return;
		}

		GameObject* camGOParent = mpCameraComponent->mpOwner->getParent();
		// Null check the camera parent in case it died.
		if (camGOParent == nullptr)
			return;

		Transform* cameraParentTransform = static_cast<Transform*>(camGOParent->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		if (cameraParentTransform == nullptr)
			return;

		float angle = mpCameraComponent->getAngle();
		float height = mpCameraComponent->getHeight();

		//std::cout << "DEBUG: " << angle << "x + " << height << std::endl;

		// Get mouse position.
		int mouseX, mouseY;
		pIM->getMousePosition(mouseX, mouseY);

		// Map -1 to 1.
		glm::vec2 mousePos = glm::vec2(
			(((float)mouseX / (float)pGM->mWindowWidth) - 0.5f) * 2.0f,
			(0.5f - ((float)mouseY / (float)pGM->mWindowHeight)) * 2.0f
		);

		//std::cout << "DEBUG: Mousepos is " << mousePos.x << ", " << mousePos.y << std::endl;

		float depth = angle * mousePos.y + height;

		/*
		std::cout << "DEBUG: " << angle << " * "<< mousePos.y << " + " << height <<
			" = " << depth << std::endl;

		*/

		//std::cout << "(" << mousePos.x << ", " << mousePos.y << ")" << std::endl;

		float zoom = GlobalManager::getGraphicsManager()->getZoomLevel();

		// Project mouse position to world coordinates.
		//auto projMtx = mpCameraComponent->getProjMatrix();
		//auto transfMtx = mpCameraComponent->getTransMatrix(); // 'view matrix'
		auto invMtx =  pCameraTransformEvent->mProjectionMatrix * pCameraTransformEvent->mViewMatrix;
		//invMtx = glm::inverse(invMtx);

		// This causes it to overshoot... No great, but good enough.
		//invMtx = glm::translate(invMtx, (mpCameraComponent->offset)  );


		glm::vec4 newposition = glm::vec4(mousePos.x * (zoom + 1.0f), mousePos.y * (zoom + 1.0f), depth, 1.0f);
		//glm::vec4 newposition = glm::vec4(mousePos.x , mousePos.y, depth, 1.0f);
		newposition = newposition * invMtx;
		newposition = newposition + glm::vec4( cameraParentTransform->getPosition(), 1.0f);


		//std::cout << "DEBUG: " << newposition.x << " vs. " << mpCameraComponent->offset.x << std::endl;

		/*
		std::cout << "DEBUG: New pos will be (" << newposition.x
			<< ", " << newposition.y << ", " << newposition.z << ", " << newposition.w << ")" << std::endl;
		*/

		//std::cout << "DEBUG: mouse z will be " << newposition.z / newposition.w << std::endl;
		mpTransform->setPosition(newposition);

		// Broadcast the new position.
		CursorToWorldCoordinatesEvent* pNewEvent = new CursorToWorldCoordinatesEvent();
		pNewEvent->mCoords = newposition;
		GlobalManager::getEventManager()->broadcastEventToSubscribers(pNewEvent);
	}
}
