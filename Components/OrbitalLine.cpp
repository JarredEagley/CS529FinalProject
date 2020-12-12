/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLLine.cpp
	Purpose:			Implementations for OrbitalLine.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/11/2020

- End Header --------------------------------------------------------*/

#include "OrbitalLine.h"
#include "glm/gtc/type_ptr.hpp"

OrbitalLine::OrbitalLine() : Component(ComponentTypes::TYPE_ORBITALLINE)
{
}

OrbitalLine::~OrbitalLine()
{
}


void OrbitalLine::Initialize()
{
}

void OrbitalLine::Update()
{
}

void OrbitalLine::handleEvent(Event* pEvent)
{
}


// Binds the uniform data associated with this paticular GLRect.
void OrbitalLine::setUniformData(ShaderProgram* pProgram)
{
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "drawType");
	glUniform1i(loc, 1); // 1 = line.
}


void OrbitalLine::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{
	// Get things we need for iterating this orbital line.
	GameObject* pParentGO = mpOwner->getParent();
	if (pParentGO == nullptr)
		return;
	PhysicsBody* pParentPhsyics = static_cast<PhysicsBody*>(pParentGO->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	if (pParentPhsyics == nullptr)
		return;

	// Bind
	unsigned int vaoID = GlobalManager::getGraphicsManager()->getVAOLine();
	glBindVertexArray(vaoID);

	// Uniforms.
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "modelTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // ModelTransf is discarded since we need world coordinates.
	loc = glGetUniformLocation(pProgram->ProgramID, "viewTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewTrans));
	loc = glGetUniformLocation(pProgram->ProgramID, "viewProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewProj));

	glm::vec2 pos = pParentPhsyics->mPosition;
	glm::vec2 vel = pParentPhsyics->mVelocity;
	glm::vec2 futurePos;
	float alpha;

	GLuint VBO = GlobalManager::getGraphicsManager()->vboIDLine[0];
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glLineWidth(mWidth);
	for (int i = 0; i < mIterations; ++i)
	{
		futurePos = pos + (vel * mStepSize);
		alpha = 1.0f-((float)i)/((float)mIterations);
		//std::cout << i << ", " << alpha << std::endl;
		mColor.a = alpha * mAlphaMultiplier;
		loc = glGetUniformLocation(pProgram->ProgramID, "color");
		glUniform4fv(loc, 1, &mColor.x);

		// Sub in my own position data.
		glm::vec4 vertPosLine[2] = {
			glm::vec4(pos, 0.0f, 1.0f),	// Left Top
			glm::vec4(futurePos, 0.0f, 1.0f)	// Left Bot
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPosLine), vertPosLine);

		// Draw.
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

		// update pos and vel at this point
		pos += vel * mStepSize;
		vel += GlobalManager::getPhysicsManager()->calculateGravitationalForces(pos) * mStepSize;
	}
	glLineWidth(1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void OrbitalLine::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Error: GLLine failed to deserialize. Value was not formatted correctly.." << std::endl;
		return;
	}
	// Get the GLRect object.
	rapidjson::GenericObject<true, rapidjson::Value> inputObj = inputMemberIt->value.GetObject();

	// Try to deserialize color.
	if (inputObj.HasMember("Color"))
	{
		if (inputObj["Color"].IsArray())
		{
			// Get the array from the JSON...
			rapidjson::GenericArray<true, rapidjson::Value> colorArray = inputObj["Color"].GetArray();

			// Check array formatting.
			if (colorArray.Capacity() == 4 && colorArray[0].IsNumber())
			{
				// Formatting is ok. Set color.
				glm::vec4 ourcolor = glm::vec4(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat(), colorArray[3].GetFloat());
				//this->setColor(ourcolor);
				this->mColor = ourcolor;
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

	if (inputObj.HasMember("Width") && inputObj["Width"].IsNumber())
		this->mWidth = inputObj["Width"].GetFloat();

	if (inputObj.HasMember("Iterations") && inputObj["Iterations"].IsNumber())
		this->mIterations = inputObj["Iterations"].GetInt();

	if (inputObj.HasMember("Step Size") && inputObj["Step Size"].IsNumber())
		this->mStepSize = inputObj["Step Size"].GetFloat();

	if (inputObj.HasMember("Alpha Multiplier") && inputObj["Alpha Multiplier"].IsNumber())
		this->mAlphaMultiplier = inputObj["Alpha Multiplier"].GetFloat();

}