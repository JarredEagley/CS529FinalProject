/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLRect.h
	Purpose:			The primary component used for graphics. This defines
						a quad, keeps track of VBO, VAO, and indices.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/1/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

#include "../Managers/GlobalManager.h"
#include "../Managers/ResourceManager.h" // I really dont like having to do this to get my encapsulation. Need to find a better way. 

#include "glm/glm.hpp"
#include "stb_image.h"

class GLRect : public Component
{
public:
	GLRect();
	~GLRect();

	void Initialize();
	void Update();

	void setColor(glm::vec4); // Sets the color of the whole square. 
	glm::vec4 getColor() { return mColor; };
	void setUvScale(float scale);
	float getUvScale();
	void setUvOffset(glm::vec2 offset);
	glm::vec2 getUvOffset();

	void setTexture(const char* imageName);

	unsigned int getVAO(); // Returns the iD of this component's vertex array object.
	int getTexId() { return mTexID; }; // Returns the ID of the texture stored on the graphics card which we want this component to draw.

	void setUniformData(ShaderProgram* pProgram);
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj);

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	void buildVAO(); // Build this component's VAO. Automatically stores vaoId.
private:
	// A 1x1 flat white square with default uv's.
	glm::vec4 mVertPos[4] = {
		// Supporting 3d transfomrations. 
		glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),		// Left Top
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),	// Left Bot
		glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),		// Right bot
		glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),		// Right top
	};
	glm::vec2 mVertUV[4] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
	};
	unsigned int mIndices[6] = {
		0, 1, 3, // First triangle.
		1, 2, 3	 // Second triangle.
	};

	GLuint vboID[3];
	unsigned int vaoID; // The OpenGL identifier for the Vertex Array Object for this gameObject.
	int mTexID; // ID of the texture we want to use; stored on the graphics card.

	glm::vec4 mColor;
	float mUvScale;
	glm::vec2 mUvOffset;
};
