/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			PhysicsBody.h
	Purpose:			A component which defines physics behaviors for
						a GameObject. This includes position, velocity,
						accleration, forces, angular acceleration, ect.
						Also responsible for collisions.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/7/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

#include "glm/glm.hpp"
#include "GL/glew.h"

class Shape;

// Roughly equivalent to the example 'body.h'.
class PhysicsBody : public Component
{
public:
	PhysicsBody();
	~PhysicsBody();

	void Initialize();
	void Update();

	void Integrate(float deltaTime);

	void calculateGravityForces(); // Applies all the gravitational force vectors.
	void applyForce(glm::vec2 F);
	void applyTorque(float T);
	
	void setTimedIgnoreCollision(PhysicsBody * pIgnored, float ignoreTime);
	PhysicsBody* getIgnoredPhysicsBody() { return this->mpIgnoredPhysicsBody; };

	bool hasGravity() { return mHasGravity; }; // hasGravity is read-only. Setter needs to inform the physics manager.
	void enableGravity();
	void disableGravity();
	void setMass(float mass); // sets mass, calculates and sets invMass.

	// For debug drawing our collision shape.

	void handleEvent(Event* pEvent);
	
	void setUniformData(ShaderProgram* pProgram);
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj);
	
	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	glm::vec2 mForwardDir;
	glm::vec2 mRightDir;

	glm::vec2 mPosition;
	glm::vec2 mPrevPosition;
	glm::vec2 mVelocity;
	glm::vec2 mAcceleration;
	glm::vec2 mTotalForce;
	float mAngle, mPrevAngle, mAngularVelocity, mAngularAcceleration, mTotalTorque;
	float mMass, mInvMass;

	Shape* mpShape; 


private:
	// No private methods.
private:
	PhysicsBody* mpIgnoredPhysicsBody;
	float mIgnorePhysicsBodyTimer;
	bool mHasGravity; 

	// Used in debug drawing:
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
	GLuint vaoID;

};

