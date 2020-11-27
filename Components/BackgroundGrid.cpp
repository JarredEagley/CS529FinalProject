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
mpGLRect(nullptr), mpTransform(nullptr)
{
}

BackgroundGrid::~BackgroundGrid()
{
}


void BackgroundGrid::Update()
{
	// Going to need zoom level.
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	// Try to set mpGLRect. This is mpowner's graphics component. (And transform)
	if (mpGLRect == nullptr || mpTransform == nullptr ) 
	{
		// Try to set mpGLRect and terminate early.
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	// TO-DO: Hijack the parenting events to make this move correctly.

	// A bit unoptimized right now.
	GameObject *pCamGO = pGM->getCurrentCameraGO();
	Camera* pCamComp = static_cast<Camera*>(pCamGO->GetComponent(ComponentTypes::TYPE_CAMERA));
	Transform* pCamTransformComp = static_cast<Transform*>(pCamGO->GetComponent(ComponentTypes::TYPE_TRANSFORM));

	glm::vec4 newpos = pCamTransformComp->getTransformationMatrix() * glm::vec4(1.0f);
	mpTransform->setPosition(newpos);

	mpTransform->setScale(pGM->getZoomLevel() + 200);

}


void BackgroundGrid::handleEvent(Event* pEvent)
{
}


void BackgroundGrid::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}