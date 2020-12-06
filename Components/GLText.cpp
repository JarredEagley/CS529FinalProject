/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLText.cpp
	Purpose:			Implementations for GLText.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/5/2020

- End Header --------------------------------------------------------*/

#include "GLText.h"
#include "../Managers/GlobalManager.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

GLText::GLText() : Component(ComponentTypes::TYPE_GLTEXT), 
mX(0.0f), mY(0.0f), mScale(0.0f)
{
	// Generate and bind our buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// No data to start.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLText::~GLText()
{
}


void GLText::Initialize()
{
}

void GLText::Update()
{
}

//void GLText::handleEvent(Event* pEvent) {};

// Used for components which draw to openGL.
void GLText::setUniformData(ShaderProgram* pProgram)
{}

void GLText::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{
	float x = mX;
	float y = mY;

	glDisable(GL_BLEND);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	//glm::mat4 projTest = glm::ortho(0.0f, 1000.0f, 0.0f, 900.0f);

	glm::mat4 testProj = glm::ortho(0.0f, 1200.0f, 0.0f, 900.0f);

	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "viewProj");
	glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	
	loc = glGetUniformLocation(pProgram->ProgramID, "viewTrans");
	glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(viewTrans));
	loc = glGetUniformLocation(pProgram->ProgramID, "modelTrans");
	glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(modelTrans));
	

	loc = glGetUniformLocation(pProgram->ProgramID, "textColor");
	glUniform3f(loc, 1.0f, 1.0f, 1.0f);

	std::cout << "'" << mTextContent << "'" << std::endl;

	// Iterate through the string.
	for (std::string::const_iterator c = mTextContent.begin(); c != mTextContent.end(); ++c)
	{
		Character ch = GlobalManager::getResourceManager()->mCharacters[*c];

		float xPos = x + ch.mBearing.x * mScale;
		float yPos = y - (ch.mSize.y - ch.mBearing.y) * mScale; // For characters like 'g' which are slightly lowered.

		float w = ch.mSize.x * mScale;
		float h = ch.mSize.y * mScale;

		std::cout << "x is " << xPos << " to " <<xPos+w << std::endl;
		std::cout << "y is " << yPos << " to " <<yPos+h << std::endl;
		std::cout << "---\n";

		// Update the VBO for each character...
		/*
		float vertices[6][4] = {
			{ xPos,     yPos + h,   0.0f, 0.0f },
			{ xPos,     yPos,       0.0f, 1.0f },
			{ xPos + w, yPos,       1.0f, 1.0f },

			{ xPos,     yPos + h,   0.0f, 0.0f },
			{ xPos + w, yPos,       1.0f, 1.0f },
			{ xPos + w, yPos + h,   1.0f, 0.0f }
		};
		*/
		
		float vertices[6][4] = {
			{ -1.0,     1.0,   0.0f, 0.0f },
			{ -1.0,     -1.0,       0.0f, 1.0f },
			{ 1.0,	   -1.0,       1.0f, 1.0f },

			{ 0.0,     1.0,   0.0f, 0.0f },
			{ 1.0, 0.0,       1.0f, 1.0f },
			{ 1.0, 1.0,   1.0f, 0.0f }
		};



		// Render glyph texture over this quad.
		glBindTexture(GL_TEXTURE_2D, ch.mTexId);
		// Update content of VBO Memory.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Draw call.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance cursor to next glyph.
		x += (ch.mAdvance >> 6) * mScale;
	}

	// Unbind.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_BLEND);
}

void GLText::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Message") && inputObj["Message"].IsString())
		this->mTextContent = inputObj["Message"].GetString();

	if (inputObj.HasMember("Position") && inputObj["Position"].IsArray())
	{
		auto arr = inputObj["Position"].GetArray();
		if (arr[0].IsNumber() && arr[1].IsNumber())
		{
			this->mX = arr[0].GetFloat();
			this->mY = arr[1].GetFloat();
		}
	}

	if (inputObj.HasMember("Scale") && inputObj["Scale"].IsNumber())
	{
		this->mScale = inputObj["Scale"].GetFloat();
	}
}


