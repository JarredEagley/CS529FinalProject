/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLRect.cpp
	Purpose:			Implementations for GLRect.h functions.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/1/2020

- End Header --------------------------------------------------------*/

#include "GLRect.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"

#include "Transform.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

GLRect::GLRect() : Component(ComponentTypes::TYPE_GLRECT),
mColor(glm::vec4(1.0f)), mUvScale(1.0f), mUvOffset(glm::vec2(0.0f))
{
}

GLRect::~GLRect()
{
}

void GLRect::Initialize() {}

void GLRect::Update()
{}


void GLRect::setColor(glm::vec4 rgba)
{
	// Store the color so we can ask GLRect for it.
	mColor = rgba;
}

void GLRect::setAlpha(float alpha)
{
	mColor.a = alpha;
}

void GLRect::setUvScale(float scale)
{
	// Keep track.
	this->mUvScale = scale;
}

float GLRect::getUvScale()
{
	return this->mUvScale;
}

void GLRect::setUvOffset(glm::vec2 offset)
{
	this->mUvOffset = offset;
}

glm::vec2 GLRect::getUvOffset()
{
	return this->mUvOffset;
}

// Asks the ResourceManager for the provided texture and applies it to this rect.
void GLRect::setTexture(const char* imageName)
{
	const std::string imagePath = GlobalManager::getResourceManager()->pathTextures + imageName;
	GLuint newTexId = GlobalManager::getResourceManager()->loadTexture(imagePath.c_str());

	if (newTexId == NULL) // Sanity checker. 
		std::cout << "Warning: GLRect texture '" << imagePath << "' failed to deserialize!" << std::endl;

	mTexID = newTexId;
}


// Binds the uniform data associated with this paticular GLRect.
void GLRect::setUniformData(ShaderProgram* pProgram)
{
	unsigned int loc;

	loc = glGetUniformLocation(pProgram->ProgramID, "uvScale");
	glUniform1f(loc, getUvScale());
	loc = glGetUniformLocation(pProgram->ProgramID, "uvOffset");
	glUniform2fv(loc, 1, &mUvOffset.x);
	loc = glGetUniformLocation(pProgram->ProgramID, "color");
	glUniform4fv(loc, 1, &mColor.x);
}


void GLRect::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{
	// Bind
	unsigned int vaoID = GlobalManager::getGraphicsManager()->getVAORect();
	glBindVertexArray(vaoID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->mTexID);

	// Uniforms.
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "drawType");
	glUniform1i(loc, 0); // 0 = rect.

	loc = glGetUniformLocation(pProgram->ProgramID, "modelTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelTrans) );
	loc = glGetUniformLocation(pProgram->ProgramID, "viewTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewTrans) );
	loc = glGetUniformLocation(pProgram->ProgramID, "viewProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewProj) );

	// Draw.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void GLRect::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	//std::cout << "DEBUG - Deserializing GLRect..." << std::endl;
	// Sanity check for serializing GLRect.
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Error: GLRect failed to deserialize. Value was not formatted correctly.." << std::endl;
		return;
	}
	// Get the GLRect object.
	rapidjson::GenericObject<true, rapidjson::Value> rectObject = inputMemberIt->value.GetObject();


	// Try to deserialize texture.
	if (rectObject.HasMember("Texture"))
	{
		if (rectObject["Texture"].IsString())
		{
			// Read in that texture.
			this->setTexture(rectObject["Texture"].GetString());
		}
		else
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: GLRect had a 'Texture' but its value was not a string!" << std::endl;
		}
	}

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
					std::cout << "Warning: GLRect had a 'Color' but its array was incorrectly formatted!" << std::endl;
			}
		}
		else
		{
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: GLRect had a 'Color' but its value was not an array of numbers!" << std::endl;
		}
	}
}