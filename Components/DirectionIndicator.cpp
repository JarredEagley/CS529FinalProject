/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			DirectionIndicator.cpp
	Purpose				Implementations for DirectionIndicator.
						Will interface with PhysicsBody and Transform.
						
						NOTE: This component's functionality grew beyond
						its original intent and is now more of a generic
						indicator component which grows with zoom.

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
	if (!mIgnoreParentColor)
	{
		if (mpOwner->getParent() == nullptr)
			return;
		GLRect* pParentRect = static_cast<GLRect*>(this->mpOwner->getParent()->GetComponent(ComponentTypes::TYPE_GLRECT));
		if (pParentRect != nullptr && this->mpGLRect != nullptr)
			this->mpGLRect->setColor(pParentRect->getColor());
	}

	// Update based on zoom...

	// Color alpha
	float const zoomLevel = pGM->getZoomLevel();
	float const maxZoomLevel = pGM->getMaxZoomLevel();
	float const minZoomLevel = pGM->getMinZoomLevel();

	float scalefactor = 1.0f; //= std::log(((zoomLevel - minZoomLevel) / (maxZoomLevel - minZoomLevel))+1.0f) * 2.0f;
	
	switch (mScaleMode)
	{
	case LOG:
		scalefactor = std::log(((zoomLevel - minZoomLevel) / (maxZoomLevel - minZoomLevel))+1.0f) * 2.0f;
		break;
	case LINEAR:
		scalefactor =  zoomLevel;
		break;
	}

	float alpha = std::min(scalefactor * mIndicatorAlphaFactor, 1.0f);
	mpGLRect->setAlpha(alpha);

	float scale = std::min(std::max(scalefactor * mIndicatorSizeFactor, mScaleMin), mScaleMax);
	
	// Scale
	mpTransform->setScale( scale );
}

void DirectionIndicator::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Alpha Factor") && inputObj["Alpha Factor"].IsNumber())
	{
		this->mIndicatorAlphaFactor = inputObj["Alpha Factor"].GetFloat();
	}

	if (inputObj.HasMember("Scale Factor") && inputObj["Scale Factor"].IsNumber())
	{
		this->mIndicatorSizeFactor = inputObj["Scale Factor"].GetFloat();
	}

	if (inputObj.HasMember("Scale Mode") && inputObj["Scale Mode"].IsString())
	{
		std::string mode = inputObj["Scale Mode"].GetString();

		if (mode == "Linear")
			this->mScaleMode = LINEAR;

		// Log is default otherwise.
	}

	if (inputObj.HasMember("Scale Min") && inputObj["Scale Min"].IsNumber())
		this->mScaleMin = inputObj["Scale Min"].GetFloat();

	if (inputObj.HasMember("Scale Max") && inputObj["Scale Max"].IsNumber())
		this->mScaleMax = inputObj["Scale Max"].GetFloat();

	if (inputObj.HasMember("Ignore Parent Color"))
		this->mIgnoreParentColor = true;

}