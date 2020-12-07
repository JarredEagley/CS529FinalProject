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

	//unsigned int getVAO(); // Returns the iD of this component's vertex array object.
	int getTexId() { return mTexID; }; // Returns the ID of the texture stored on the graphics card which we want this component to draw.

	void setUniformData(ShaderProgram* pProgram);
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj);

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:

private:
	int mTexID; // ID of the texture we want to use; stored on the graphics card.

	glm::vec4 mColor;
	float mUvScale;
	glm::vec2 mUvOffset;
};
