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
//#include <unordered_map>

#include "Components/Component.h"
#include "Components/ComponentTypes.h"

#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/ControllerSlider.h"
#include "Components/UpDown.h"

GameObject::GameObject()
{
	if (NULL == maxId)
	{
		maxId = 0;
		this->gameObjectID = maxId;
	}
	else
	{
		++maxId;
		this->gameObjectID = maxId;
	}
}

GameObject::~GameObject()
{
	for (auto pComponentPair : mComponents)
		delete pComponentPair.second;

	mComponents.clear();
}

void GameObject::Update()
{
	for (auto pComponentPair : mComponents)
		pComponentPair.second->Update();
}

Component* GameObject::AddComponent(unsigned int Type) 
{
	Component* pNewComponent;
	pNewComponent = GetComponent(Type); // test if it already exists.
	if (pNewComponent != nullptr)
		return pNewComponent;

	// If it doesn't, just run overwriteComponent.
	return overwriteComponent(Type);
}

// Adds a component. If the component already exists, it will be overwritten.
// Also acts as the foundation for addComponent.
Component* GameObject::overwriteComponent(unsigned int Type) // TO-DO: OverwriteComponent should probably be removed in the future.
{
	// If it already exists, delete.
	Component* pNewComponent;
	pNewComponent = GetComponent(Type);
	if (pNewComponent != nullptr)
	{
		// Delete the current component of this type, then continue on.
		delete pNewComponent;
	}

	// Component doesn't exist yet. Create it.
	switch (Type)
	{
	case(ComponentTypes::TYPE_UNDEFINED):
		std::cout << "Warning: Adding GameObject Component failed; TYPE_UNDEFINED" << std::endl;
		return nullptr; // Failed to create component.
	case (ComponentTypes::TYPE_TRANSFORM):
		pNewComponent = new Transform();
		break;
	case (ComponentTypes::TYPE_SPRITE):
		pNewComponent = new Sprite();
		break;
	case (ComponentTypes::TYPE_CONTROLLERSLIDER):
		pNewComponent = new ControllerSlider();
		break;
	case (ComponentTypes::TYPE_UPDOWN):
		pNewComponent = new UpDown();
		break;
	default:
		return nullptr; // Failed to create component.
	}

	// Originally there was a nullcheck here.
	// It seemed useless... We'll see if I was wrong.

	// Add to list
	//mComponents.push_back(pNewComponent);
	mComponents[Type] = pNewComponent;
	pNewComponent->mpOwner = this;

	// Return the component.
	return pNewComponent;
}

Component* GameObject::GetComponent(unsigned int Type)
{
	for (auto pComponentPair : mComponents)
		if (pComponentPair.second->getType() == Type)
			return pComponentPair.second;
		/*
		if (pComponent->getType() == Type)
			return pComponent;
		*/

	return nullptr;
}