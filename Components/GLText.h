/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GLText.h
	Purpose:			Implements text rendering using freetype, using
						learnopengl.com's guide.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/5/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "glm/glm.hpp"
#include <map>



class GLText : public Component
{
public:
	GLText();
	~GLText();

	void Initialize();
	void Update(); 
	//void handleEvent(Event* pEvent) {};

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	

private:
	// No private methods.
private:
	// No private variables.
};

