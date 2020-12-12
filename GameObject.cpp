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

#include "Components/ControllerShip.h"

#include "Components/Turret.h"
#include "Components/MissileLauncher.h"

#include "Components/AIMissile.h"
#include "Components/AIEnemyCore.h"
#include "Components/AIEnemyStationary.h"
#include "Components/AIEnemyChase.h"

#include "Components/GLRect.h"
#include "Components/GLText.h"
#include "Components/GLLine.h"

#include "Components/FXPlume.h"

#include "Components/Camera.h"
#include "Components/BackgroundGrid.h"
#include "Components/PhysicsBody.h"
#include "Components/DirectionIndicator.h"
#include "Components/OrbitalLine.h"

#include "Components/ShipData.h"
#include "Components/ThrottleMeter.h"
#include "Components/FuelMeter.h"
#include "Components/HealthMeter.h"
#include "Components/FollowCursor.h"
#include "Components/Explosion.h"
#include "Components/NPCHealthMeter.h"
#include "Components/MenuItem.h"


GameObject::GameObject() : mName(""),
mpParentGO(nullptr), mParentGOName(""),
mIsParented(false), mIsAlive(false), mIsMarkedForDelete(false),
mRenderPassType(RenderPassType::NONE)
{
	if (GlobalManager::getGameStateManager()->DEBUG_VerboseGameObjects)
		std::cout << "Constructing new GameObject..." << std::endl;
	std::unordered_map<unsigned int, Component*> mComponents;
}

GameObject::~GameObject()
{
	if (mName == "PLAYER" && this->mIsAlive && !GlobalManager::getGameStateManager()->mIsGamePaused)
	{
		// Didn't end up needing this player killed event.
		PlayerKilledEvent *pKillEvent = new PlayerKilledEvent();
		GlobalManager::getEventManager()->broadcastEvent(pKillEvent);

		GlobalManager::getGameStateManager()->isPlayerKilled = true;
	}

	if (GlobalManager::getGameStateManager()->DEBUG_VerboseGameObjects)
		std::cout << "Destructor called on " << mName << std::endl;
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
	for (auto pChildGO : mChildren)
	{
		if (pChildGO != nullptr)
		{
			pChildGO->mpParentGO = nullptr; // NEED to notify child that parent is now gone.
			// Also pass down a gameobject destroyed event from the parent to the child.
			pChildGO->handleEvent(pDestroyEvent);
		}
	}
}

void GameObject::initializeComponents()
{
	if (GlobalManager::getGameStateManager()->DEBUG_VerboseGameObjects)
		std::cout << "Initializing GameObject " << this->mName << std::endl;
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

	// If we're not alive yet, just stop here.
	if (!mIsAlive)
		return;

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

	case (ComponentTypes::TYPE_CONTROLLERSHIP):
		pNewComponent = new ControllerShip();
		break;
	case (ComponentTypes::TYPE_TURRET):
		pNewComponent = new Turret();
		break;
	case (ComponentTypes::TYPE_MISSILELAUNCHER):
		pNewComponent = new MissileLauncher();
		break;

	case (ComponentTypes::TYPE_AI_MISSILE):
		pNewComponent = new AIMissile();
		break;
	case (ComponentTypes::TYPE_AI_CORE):
		pNewComponent = new AIEnemyCore();
		break;
	case (ComponentTypes::TYPE_AI_STATIONARY):
		pNewComponent = new AIEnemyStationary();
		break;
	case (ComponentTypes::TYPE_AI_CHASE):
		pNewComponent = new AIEnemyChase();
		break;

	case (ComponentTypes::TYPE_GLRECT):
		pNewComponent = new GLRect();
		break;
	case (ComponentTypes::TYPE_GLLINE):
		pNewComponent = new GLLine();
		break;
	case (ComponentTypes::TYPE_GLTEXT):
		pNewComponent = new GLText();
		break;
		
	case (ComponentTypes::TYPE_FX_PLUME):
		pNewComponent = new FXPlume();
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
	case (ComponentTypes::TYPE_ORBITALLINE):
		pNewComponent = new OrbitalLine();
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
	case (ComponentTypes::TYPE_METER_HEALTH):
		pNewComponent = new HealthMeter();
		break;
	case (ComponentTypes::TYPE_MENUITEM):
		pNewComponent = new MenuItem();
		break;
	case (ComponentTypes::TYPE_FOLLOWCURSOR):
		pNewComponent = new FollowCursor();
		break;
	case (ComponentTypes::TYPE_EXPLOSION):
		pNewComponent = new Explosion();
		break;
	case (ComponentTypes::TYPE_METER_HEALTH_NPC):
		pNewComponent = new NPCHealthMeter();
		break;


	default:
		return nullptr; // Failed to create component.
	}

	if (pNewComponent == nullptr)
		return nullptr;

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

void GameObject::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{
	for (auto pCompPair : mComponents)
		pCompPair.second->Draw(pProgram, modelTrans, viewTrans, viewProj);
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
			//std::cout << "GameObject " << mName << " destroyed by parent object " << pGODestroyedEvent->mDestroyedGOName << std::endl;
			//GlobalManager::getGameObjectManager()->mMarkedForDelete.push_back(this->mName);
			this->mIsMarkedForDelete = true;
		}
	}

	// Normal game objects will never be handed a destroy projectile event.
	if (pEvent->mType == EventType::DESTROY_PROJETILE)
	{
		//GlobalManager::getGameObjectManager()->mMarkedForDelete.push_back(this->mName);
		this->mIsMarkedForDelete = true;
	}
}