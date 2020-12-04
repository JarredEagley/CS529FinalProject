/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GameObject.h
	Purpose:			The header file for GameObject.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/13/2020

- End Header --------------------------------------------------------*/

#pragma once

#include <unordered_map>

class Component;
class Event;

class ShaderProgram;

enum class RenderPassType;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void initializeComponents();

	void setParent(std::string parentGOName);
	GameObject* getParent();

	Component* AddComponent(unsigned int Type); // Returns the component which was allocated.
	Component* GetComponent(unsigned int Type); // Because we need to access components.

	// One object may only be in one render pass.
	// May not be ideal but for my applications its fine.
	void setRenderPass(RenderPassType renderPassType);
	RenderPassType getRenderPassType(); // returns render pass enum

	void setAllUniformData(ShaderProgram* pProgram); // Calls setUniformData on all this GO's components.

	void handleEvent(Event *pEvent);
public:
	bool mHasChildren;
	std::unordered_map<unsigned int, Component*> mComponents; // <TYPE, COMPONENT>
	std::string mShaderName = "core";	// May have more shaders in the future.
	std::string mName;
	RenderPassType mRenderPassType; // render pass type enum
private:
	bool mIsParented;
	std::string mParentGOName; // Name of the GO we'll try to get for parenting.
	GameObject* mpParentGO;
private:
	// No private variables.
};

