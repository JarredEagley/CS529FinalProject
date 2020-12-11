/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FXPlume.h
	Purpose				A simple gameobject which when attached to a ship
						listens for shipdata and displays a pretty engine
						plume when the main drive is spooled up.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/10/2020

- End Header --------------------------------------------------------*/


#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "GLRect.h"

class FXPlume : public Component
{
public:
	FXPlume();
	~FXPlume();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram) {};
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj) {};

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private methods.
private:
	GLRect* pGLRect = nullptr;

};

