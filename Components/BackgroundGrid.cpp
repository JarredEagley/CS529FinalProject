/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			BackgroundGrid.cpp
	Purpose				Implements the functionality from BackgroundGrid.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/27/2020

- End Header --------------------------------------------------------*/

#include "BackgroundGrid.h"
#include "../Managers/GlobalManager.h"

BackgroundGrid::BackgroundGrid() : Component(ComponentTypes::TYPE_BACKGROUNDGRID),
mpGLRect(nullptr), mpTransform(nullptr), mGridScale(1000.0f), mGridIntensity(0.8f)
{
}

BackgroundGrid::~BackgroundGrid()
{
}


void BackgroundGrid::Initialize() 
{
	GlobalManager::getEventManager()->Subscribe(EventType::CAMERA_TRANSFORM_UPDATED, this->mpOwner);
	GlobalManager::getEventManager()->Subscribe(EventType::MOUSE_SCROLL, this->mpOwner);
}

void BackgroundGrid::Update()
{
	// Background grid will be updated by events rather than by its game object.
	// Pseudo-parented to camera.
}

void BackgroundGrid::transformEventUpdate(Event* pEvent)
{
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	// Listen for camera to update its transformation matrix.
	CameraTransformUpdatedEvent* pCamEvent = static_cast<CameraTransformUpdatedEvent*>(pEvent);

	// Try to set mpGLRect. This is mpowner's graphics component. (And transform)
	if (mpGLRect == nullptr || mpTransform == nullptr)
	{
		// Try to set mpGLRect and terminate early.
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	// A bit unoptimized right now.
	GameObject* pCamGO = pGM->getCurrentCameraGO();
	Camera* pCamComp = static_cast<Camera*>(pCamGO->GetComponent(ComponentTypes::TYPE_CAMERA));
	Transform* pCamTransformComp = static_cast<Transform*>(pCamGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));

	glm::vec4 newpos = pCamTransformComp->getTransformationMatrix() * glm::vec4(1.0f) + glm::vec4(pCamComp->offset, 1.0f);
	mpTransform->setPosition(newpos);

	mpGLRect->setUvScale(pGM->getZoomLevel() / (mGridScale * 2.0f));
	mpGLRect->setUvOffset(newpos / (pGM->getZoomLevel() * 2.0f));
	mpTransform->setScale( (pGM->getZoomLevel() + mGridScale) * 2.0f );
}

// Separated out for potential future optimizations.
void BackgroundGrid::scrollEventUpdate(Event* pEvent)
{
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	// Listen for camera to update its transformation matrix.
	CameraTransformUpdatedEvent* pCamEvent = static_cast<CameraTransformUpdatedEvent*>(pEvent);

	// Try to set mpGLRect. This is mpowner's graphics component. (And transform)
	if (mpGLRect == nullptr || mpTransform == nullptr)
	{
		// Try to set mpGLRect and terminate early.
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	// A bit unoptimized right now.
	GameObject* pCamGO = pGM->getCurrentCameraGO();
	Camera* pCamComp = static_cast<Camera*>(pCamGO->GetComponent(ComponentTypes::TYPE_CAMERA));
	Transform* pCamTransformComp = static_cast<Transform*>(pCamGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));

	glm::vec4 newpos = pCamTransformComp->getTransformationMatrix() * glm::vec4(1.0f) + glm::vec4(pCamComp->offset, 1.0f);
	mpTransform->setPosition(newpos);

	mpGLRect->setUvScale(pGM->getZoomLevel() / (mGridScale * 2.0f) );
	mpGLRect->setUvOffset(newpos / (pGM->getZoomLevel() * 2.0f) );
	mpTransform->setScale((pGM->getZoomLevel() + mGridScale) * 2.0f);

	float const scalefactor = std::log(((pGM->getZoomLevel() - pGM->getMinZoomLevel()) / (pGM->getMaxZoomLevel() - pGM->getMinZoomLevel())) + 1.0f) * 2.0f;
	float const newAlpha = std::min(scalefactor, mGridIntensity);
	mpGLRect->setColor(   glm::vec4(  glm::vec3(mpGLRect->getColor()), newAlpha   )   );

}

void BackgroundGrid::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::CAMERA_TRANSFORM_UPDATED)
	{
		transformEventUpdate(pEvent);
	}
	if (pEvent->mType == EventType::MOUSE_SCROLL)
	{
		scrollEventUpdate(pEvent);
	}
}


void BackgroundGrid::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}