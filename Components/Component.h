/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Component.h
	Purpose:			Header file for the abstract Component class.
						The only function here which is directly implemented
						is the constructor.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/19/2020

- End Header --------------------------------------------------------*/

#pragma once

//#include <fstream>
#include "rapidjson/document.h"
#include "glm/mat4x4.hpp"

class GameObject; 
class Event;
class ShaderProgram;

// Abstract class because of Update() = 0...
class Component
{
public:
	Component(unsigned int Type);
	virtual ~Component() {}; 

	//virtual void Update() {}; // This would allow empty update functions
	virtual void Initialize() = 0;
	virtual void Update() = 0; // This would be a pure virtual function which would make Component an abstract class. Because it has one or more pure virt functs
	virtual void handleEvent(Event* pEvent) {};
	
	// Used for components which draw to openGL.
	virtual void setUniformData(ShaderProgram* pProgram) {};
	virtual void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj) {};

	unsigned int getType() { return mType; };

	//virtual void Serialize(std::ifstream& InputStream) = 0;  
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) = 0;  
public:
	GameObject* mpOwner; // Should be private with a setter TO-DO
private:
private:
	unsigned int mType;
};

