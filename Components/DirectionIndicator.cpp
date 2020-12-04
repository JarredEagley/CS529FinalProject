/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			DirectionIndicator.cpp
	Purpose				Implementations for DirectionIndicator.
						Will interface with PhysicsBody and Transform.
						May eventually track rotation angle as well.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/25/2020

- End Header --------------------------------------------------------*/

#include "DirectionIndicator.h"
#include "ComponentTypes.h"
#include "Transform.h"
#include "../Managers/GlobalManager.h"

DirectionIndicator::DirectionIndicator() : Component(ComponentTypes::TYPE_DIRINDICATOR), 
mpGLRect(nullptr), mpTransform(nullptr)
{
}

DirectionIndicator::~DirectionIndicator()
{
}


void DirectionIndicator::Initialize() {}

void DirectionIndicator::Update() 
{
	// Need graphics manager for the zoom level.
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	// Try to set mpGLRect. This is mpowner's graphics component. (And transform)
	if (mpGLRect == nullptr || mpTransform == nullptr)
	{
		// Try to set mpGLRect and terminate early.
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		return;
	}

	// Set color to owner's color.
	if (mpOwner->getParent() == nullptr)
		return;
	GLRect* pParentRect = static_cast<GLRect*>(this->mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_GLRECT));
	if (pParentRect != nullptr && this->mpGLRect != nullptr)
		this->mpGLRect->setColor(pParentRect->getColor());

	// Update based on zoom...

	// Color alpha
	glm::vec4 color = mpGLRect->getColor();
	float const zoomLevel = pGM->getZoomLevel();
	float const maxZoomLevel = pGM->getMaxZoomLevel();
	float const minZoomLevel = pGM->getMinZoomLevel();
	float const scalefactor = std::log(((zoomLevel - minZoomLevel) / (maxZoomLevel - minZoomLevel))+1.0f) * 2.0f;
	//std::cout << "DEBUG: scalefactor is " << scalefactor << std::endl;
	color.a = scalefactor * mIndicatorAlphaFactor;
	//std::cout << "DEBUG: Alpha is " << color.a << std::endl;
	mpGLRect->setColor(color);

	// Scale
	mpTransform->setScale( scalefactor * mIndicatorSizeFactor);
}

void DirectionIndicator::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
}