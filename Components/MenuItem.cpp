/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			MenuItem.c[[
	Purpose				Implementations for MenuItem.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/8/2020

- End Header --------------------------------------------------------*/

#include "MenuItem.h"
#include "../Managers/GlobalManager.h"


MenuItem::MenuItem() : Component(ComponentTypes::TYPE_MENUITEM)
{

}

MenuItem::~MenuItem()
{
}

void MenuItem::Initialize()
{
}


void MenuItem::Update()
{
	if (mpTransform == nullptr || mpGLRect == nullptr)
	{
		mpTransform = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		mpGLRect = static_cast<GLRect*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLRECT));
		return;
	}

	InputManager* pIM = GlobalManager::getInputManager();
	GraphicsManager* pGM = GlobalManager::getGraphicsManager();

	// Get mouse coordinates.
	int pX, pY;
	pIM->getMousePosition(pX, pY);
	float mouseX = ((float)pX / (float)pGM->mWindowWidth);
	float mouseY = ((float)pY / (float)pGM->mWindowHeight);

	float scaleX = mpTransform->getScale().x;
	float scaleY = mpTransform->getScale().y;
	glm::vec2 pos = mpTransform->getPosition();


}

void MenuItem::handleEvent(Event* pEvent)
{

}

void MenuItem::setUniformData(ShaderProgram* pProgram) {}

void MenuItem::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) {}