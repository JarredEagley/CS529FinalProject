/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameObject.cpp
	Purpose:			Handles functionalities associated with GameObject.
						This entails managing and updating components.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#include "GameObject.h"
#include <iostream>

#include "Components/Component.h"
#include "Components/ComponentTypes.h"

#include "Components/Transform.h"
#include "Components/Timer.h"
#include "Components/ControllerSlider.h"
#include "Components/ControllerShip.h"
#include "Components/UpDown.h"
#include "Components/GLRect.h"
#include "Components/Camera.h"
#include "Components/BackgroundGrid.h"
#include "Components/PhysicsBody.h"
#include "Components/DirectionIndicator.h"
#include "Components/ShipData.h"
#include "Components/HUDElement.h"
#include "Components/ThrottleMeter.h"
// #include "Components/FuelMeter.h"


GameObject::GameObject() : mName(""), mpParentGO(nullptr), mHasChildren(false),
mRenderPassType(RenderPassType::NONE)
{
	std::unordered_map<unsigned int, Component*> mComponents;
}

GameObject::~GameObject()
{
	// Remove all my components.
	for (auto pComponentPair : mComponents)
		delete pComponentPair.second;
	mComponents.clear();

	// Remove from render passes.
	GlobalManager::getGraphicsManager()->removeFromAnyRenderPasses(this);
}

void GameObject::initializeComponents()
{
	for (auto pComponentPair : mComponents)
		pComponentPair.second->Initialize();
}

void GameObject::Update()
{
	// Update components.
	for (auto pComponentPair : mComponents)
		pComponentPair.second->Update();
}


void GameObject::setParent(GameObject* pParentGO)
{
	// Nullcheck.
	if (pParentGO == nullptr)
		return;

	// Set the parent.
	this->mpParentGO = pParentGO;

	// Tell parent it has children.
	pParentGO->mHasChildren = true;

	// Subscribe me to transform updates.
	GlobalManager::getEventManager()->Subscribe(EventType::TRANSFORM_UPDATED, this);
}

GameObject* GameObject::getParent()
{
	if (this->mpParentGO == nullptr)
		return nullptr;
	return this->mpParentGO;
}


Component* GameObject::AddComponent(unsigned int Type) 
{
	Component* pNewComponent;
	pNewComponent = GetComponent(Type); // test if it already exists.
	if (pNewComponent != nullptr)
		return pNewComponent; // If it does, return it.

	// Component doesn't exist yet. Create it.
	switch (Type)
	{
	case(ComponentTypes::TYPE_UNDEFINED):
		std::cout << "Warning: Adding GameObject Component failed; TYPE_UNDEFINED" << std::endl;
		return nullptr; // Failed to create component.
	case (ComponentTypes::TYPE_TRANSFORM):
		pNewComponent = new Transform();
		break;
	case (ComponentTypes::TYPE_TIMER):
		pNewComponent = new Timer();
		break;
	case (ComponentTypes::TYPE_CONTROLLERSLIDER):
		pNewComponent = new ControllerSlider();
		break;
	case (ComponentTypes::TYPE_CONTROLLERSHIP):
		pNewComponent = new ControllerShip();
		break;
	case (ComponentTypes::TYPE_UPDOWN):
		pNewComponent = new UpDown();
		break;
	case (ComponentTypes::TYPE_GLRECT):
		pNewComponent = new GLRect();
		break;
	case (ComponentTypes::TYPE_CAMERA):
		pNewComponent = new Camera();
		break;
	case (ComponentTypes::TYPE_BACKGROUNDGRID):
		pNewComponent = new BackgroundGrid();
		break;
	case (ComponentTypes::TYPE_PHYSICSBODY):
		pNewComponent = new PhysicsBody();
		break;
	case (ComponentTypes::TYPE_DIRINDICATOR):
		pNewComponent = new DirectionIndicator();
		break;
	case (ComponentTypes::TYPE_SHIPDATA):
		pNewComponent = new ShipData();
		break;
	case (ComponentTypes::TYPE_HUDELEMENT):
		pNewComponent = new HUDElement();
		break;
	case (ComponentTypes::TYPE_MARKER_THROTTLE):
		pNewComponent = new ThrottleMeter();
		break;
	

	default:
		return nullptr; // Failed to create component.
	}

	// Originally there was a nullcheck here.
	// It seemed useless... We'll see if I was wrong.

	// Add to list
	pNewComponent->mpOwner = this;
	mComponents[Type] = pNewComponent;

	// Return the component.
	return pNewComponent;
}


Component* GameObject::GetComponent(unsigned int Type)
{
	for (auto pComponentPair : mComponents)
		if (pComponentPair.second->getType() == Type)
			return pComponentPair.second;

	return nullptr;
}


void GameObject::setRenderPass(RenderPassType renderPassType)
{
	GlobalManager::getGraphicsManager()->addToRenderPass(this, renderPassType);
}

RenderPassType GameObject::getRenderPassType()
{
	return this->mRenderPassType;
}


void GameObject::handleEvent(Event* pEvent)
{
	// Hand the event down to the components.
	for (auto pComponentPair : mComponents)
	{
		pComponentPair.second->handleEvent(pEvent);
	}
}