/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLLine.h
	Purpose:			Similar to GLLine, but works iteratively to draw
						a curved line showing the player's predicted trajectory.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "GLLine.h"
#include "PhysicsBody.h"

class OrbitalLine : public Component
{
public:
	OrbitalLine();
	~OrbitalLine();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram);
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	int mIterations = 10;
	float mStepSize = 100.0f;

	float mWidth = 1.0f;
	glm::vec4 mColor;
	float mAlphaMultiplier = 1.0f;

private:
	// No private methods.
private:
	//GLLine* mpLine = nullptr;

};

