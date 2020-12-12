/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLLine.cpp
	Purpose:			Implementations for GLLine.h functions.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/9/2020

- End Header --------------------------------------------------------*/

#include "GLLine.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"

#include "Transform.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

GLLine::GLLine() : Component(ComponentTypes::TYPE_GLRECT)
{
}

GLLine::~GLLine()
{
}

void GLLine::Initialize() {}

void GLLine::Update()
{}


void GLLine::setColor(glm::vec4 rgba)
{
	// Store the color so we can ask GLRect for it.
	mColor = rgba;
}

void GLLine::setAlpha(float alpha)
{
	mColor.a = alpha;
}


// Binds the uniform data associated with this paticular GLRect.
void GLLine::setUniformData(ShaderProgram* pProgram)
{
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "color");
	glUniform4fv(loc, 1, &mColor.x);

	loc = glGetUniformLocation(pProgram->ProgramID, "drawType");
	glUniform1i(loc, 1); // 1 = line.
}


void GLLine::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{
	// Bind
	unsigned int vaoID = GlobalManager::getGraphicsManager()->getVAORect();
	glBindVertexArray(vaoID);

	// Uniforms.
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "modelTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelTrans) );
	loc = glGetUniformLocation(pProgram->ProgramID, "viewTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewTrans) );
	loc = glGetUniformLocation(pProgram->ProgramID, "viewProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewProj) );

	// Sub in my own position data.
	glm::vec4 vertPosLine[2] = {
		glm::vec4(mStartPos, 0.0f, 1.0f),	// Left Top
		glm::vec4(mEndPos, 0.0f, 1.0f)	// Left Bot
	};
	GLuint VBO = GlobalManager::getGraphicsManager()->vboIDLine[0];
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPosLine), vertPosLine);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Draw.
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void GLLine::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	//std::cout << "DEBUG - Deserializing GLRect..." << std::endl;
	// Sanity check for serializing GLRect.
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Error: GLLine failed to deserialize. Value was not formatted correctly.." << std::endl;
		return;
	}
	// Get the GLRect object.
	rapidjson::GenericObject<true, rapidjson::Value> rectObject = inputMemberIt->value.GetObject();

	// Try to deserialize color.
	if (rectObject.HasMember("Color"))
	{
		if (rectObject["Color"].IsArray())
		{
			// Get the array from the JSON...
			rapidjson::GenericArray<true,rapidjson::Value> colorArray = rectObject["Color"].GetArray();

			// Check array formatting.
			if (colorArray.Capacity() == 4 && colorArray[0].IsNumber())
			{
				// Formatting is ok. Set color.
				glm::vec4 ourcolor = glm::vec4(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat(), colorArray[3].GetFloat());
				this->setColor(ourcolor);
			}
			else
			{
				if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
					std::cout << "Warning: GLLine had a 'Color' but its array was incorrectly formatted!" << std::endl;
			}
		}
		else
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: GLLine had a 'Color' but its value was not an array of numbers!" << std::endl;
		}
	}
}