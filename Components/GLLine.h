/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLLine.h
	Purpose:			A secondary drawing component-- this one draws a
						line. Useful for trajectories, helping the player
						visualise their velocity, ect.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

#include "../Managers/GlobalManager.h"

#include "glm/glm.hpp"

class GLLine : public Component
{
public:
	GLLine();
	~GLLine();

	void Initialize();
	void Update();

	void setColor(glm::vec4); // Sets the color of the whole square. 
	glm::vec4 getColor() { return mColor; };
	void setAlpha(float alpha);

	void setUniformData(ShaderProgram* pProgram);
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj);

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	glm::vec2 mStartPos = glm::vec2(1.0f);
	glm::vec2 mEndPos = glm::vec2(0.0f);

private:
	// No private variables.
private:
	glm::vec4 mColor;

};
