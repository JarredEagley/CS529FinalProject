/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameObject.cpp
	Purpose:			Handles functionalities associated with GameObject.
						This entails managing and updating components.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
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
#include "Components/GLRect.h"
#include "Components/Camera.h"
#include "Components/BackgroundGrid.h"
#include "Components/PhysicsBody.h"
#include "Components/DirectionIndicator.h"
#include "Components/ShipData.h"
#include "Components/ThrottleMeter.h"
#include "Components/FuelMeter.h"
#include "Components/Turret.h"
#include "Components/FollowCursor.h"

GameObject::GameObject() : mName(""), 
mpParentGO(nullptr), mParentGOName(""), 
mIsParented(false),
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


	// If I have a parent, remove from parent's mChildren.
	if (mpParentGO != nullptr)
		mpParentGO->mChildren.remove(this);

	// Remove as a subscriber from anything.
	GlobalManager::getEventManager()->UnsubscribeAll(this);


	// Broadcast desctruction to child GameObjects.
	// Note: Camera GO will ignore this event. Don't want to kill the camera.
	GameObjectDestroyedEvent* pDestroyEvent = new GameObjectDestroyedEvent(mName);
	for (auto pGO : mChildren)
	{
		if (pGO != nullptr)
		{
			pGO->mpParentGO = nullptr; // NEED to notify child that parent is now gone.
			// Also pass down a gameobject destroyed event from the parent to the child.
			pGO->handleEvent(pDestroyEvent);
		}
	}
}

void GameObject::initializeComponents()
{
	for (auto pComponentPair : mComponents)
		pComponentPair.second->Initialize();
}

void GameObject::Update()
{
	// Check parent.
	if (mIsParented && mpParentGO == nullptr)
	{
		setParent(mParentGOName);
	}

	// Update components.
	for (auto pComponentPair : mComponents)
		pComponentPair.second->Update();
}


void GameObject::setParent(std::string parentGOName)
{
	// Set true so we can continue trying to aquire parent if we failed.
	this->mIsParented = true;
	this->mParentGOName = parentGOName;

	// Get the GameObject and check if it exists.
	GameObject* pParentGO = GlobalManager::getGameObjectManager()->getGameObject(parentGOName);
	if (pParentGO == nullptr)
	{
		// Cannot proceed if it doesn't exist... 
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseGameObjects)
			std::cout << "GameObject '" << this->mName << "' failed to set parent to '" << parentGOName << "'. " << std::endl;
		return;
	}

	// Set the parent.
	this->mpParentGO = pParentGO;

	// Tell parent it has children.
	pParentGO->mChildren.push_back(this);

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
	case (ComponentTypes::TYPE_MARKER_THROTTLE):
		pNewComponent = new ThrottleMeter();
		break;
	case (ComponentTypes::TYPE_METER_FUEL):
		pNewComponent = new FuelMeter();
		break;
	case (ComponentTypes::TYPE_TURRET):
		pNewComponent = new Turret();
		break;
	case (ComponentTypes::TYPE_FOLLOWCURSOR):
		pNewComponent = new FollowCursor();
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


void GameObject::setAllUniformData(ShaderProgram* pProgram)
{
	for (auto pCompPair : mComponents)
		pCompPair.second->setUniformData(pProgram); // Will be empty call for most components.
}


void GameObject::handleEvent(Event* pEvent)
{
	// Hand the event down to the components.
	for (auto pComponentPair : mComponents)
	{
		pComponentPair.second->handleEvent(pEvent);
	}

	if (pEvent->mType == EventType::GAMEOBJECT_DESTROYED)
	{
		// Make sure it was our parent GO getting destroyed.
		auto pGODestroyedEvent = static_cast<GameObjectDestroyedEvent*>(pEvent);
		if (pGODestroyedEvent->mDestroyedGOName != mParentGOName)
			return;

		if (GlobalManager::getGraphicsManager()->getCurrentCameraGO() == this)
		{
			// Camera GO ignores destruction, but maybe apply parent's transform while you still can?
		}
		else
		{
			//GlobalManager::getGameObjectManager()->mMarkedForDelete.push_back(this->mName);
		}
	}

	// Normal game objects will never be handed a destroy projectile event.
	if (pEvent->mType == EventType::DESTROY_PROJETILE)
	{
		GlobalManager::getGameObjectManager()->mMarkedForDelete.push_back(this->mName);
	}
}