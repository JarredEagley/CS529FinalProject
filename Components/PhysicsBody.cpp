/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			PhysicsBody.cpp
	Purpose:			implementations for PhysicsBody.h.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/7/2020

- End Header --------------------------------------------------------*/

#include "PhysicsBody.h"
#include "Transform.h"
#include "ComponentTypes.h"
#include "../GameObject.h"
#include "../Managers/GlobalManager.h"

#define G_CONST 0.0000000000667 // The universal gravitational constant. in N * m^2 / kg^2

PhysicsBody::PhysicsBody() : Component(ComponentTypes::TYPE_PHYSICSBODY),
mHasGravity(false),
mPosition(glm::vec2(0.0f)), mAngle(0.0f),
mPrevPosition(glm::vec2(0.0f)), mPrevAngle(0.0f),
mVelocity(glm::vec2(0.0f)), mAngularVelocity(0.0f),
mAcceleration(glm::vec2(0.0f)), mAngularAcceleration(0.0f),
mTotalForce(glm::vec2(0.0f)), mTotalTorque(0.0f),
mMass(1.0f), mInvMass(1.0f),
mForwardDir(glm::vec2(0.0f,1.0f)), mRightDir(glm::vec2(1.0f,0.0f)),
mpShape(nullptr), mpIgnoredPhysicsBody(nullptr), mIgnorePhysicsBodyTimer(0.0f),
mCollisionType(collisionType::NORMAL)
{
}

PhysicsBody::~PhysicsBody()
{
	if (mpShape != nullptr)
		delete mpShape;
}


void PhysicsBody::Initialize() {}

void PhysicsBody::Update()
{
	if (mpIgnoredPhysicsBody != nullptr)
	{
		if (mIgnorePhysicsBodyTimer > 0.0f)
		{
			mIgnorePhysicsBodyTimer -= GlobalManager::getPhysicsManager()->getGameTime();
		}
		else
		{
			mpIgnoredPhysicsBody = nullptr;
		}
	}
}


void PhysicsBody::Integrate(float deltaTime)
{
	calculateGravityForces();

	// Get the associated transform. 
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // Probably shouldn't get this EVERY integration. Oh well.

	if (nullptr != pT)
	{
		mPosition = glm::vec2(pT->getPosition());
		mAngle = pT->getRotation();
	}

	mPrevPosition = mPosition;
	mPrevAngle = mAngle;

	// Compute acceleration.
	// Normally global gravity would be here on this line.
	mAcceleration = mTotalForce * mInvMass;
	mAngularAcceleration = mTotalTorque * mInvMass;

	// Clear force and torque.
	mTotalForce = glm::vec2(0.0f);
	mTotalTorque = 0.0f;

	// Integrate velocity. V1 = a*t + V0
	mVelocity = mAcceleration * deltaTime + mVelocity;
	mAngularVelocity = mAngularAcceleration * deltaTime + mAngularVelocity;


	// Integrate position. P1 = V1*t + P0
	mPosition = mVelocity * deltaTime + mPosition;
	mAngle = mAngularVelocity * deltaTime + mAngle;


	// Compute new direction vectors.
	mForwardDir = glm::vec2(sin(glm::radians(-mAngle)), cos(glm::radians(mAngle)));
	mRightDir = glm::vec2( cos(glm::radians(mAngle)), sin(glm::radians(mAngle)) );


	if (nullptr != pT)
	{
		pT->setPosition(mPosition);
		pT->setRotation(mAngle);
	}

	//std::cout << "DEBUG - Physics integration position is (" << mPosition.x << ", " << mPosition.y << ")" << std::endl;
}


void PhysicsBody::calculateGravityForces()
{
	for (auto pBody : GlobalManager::getPhysicsManager()->gravityBodies)
	{
		if (this == pBody)
			continue;

		double numerator = (G_CONST * this->mMass * pBody->mMass);

		double denominator = (pBody->mPosition.x - this->mPosition.x) * (pBody->mPosition.x - this->mPosition.x)
			+ (pBody->mPosition.y - this->mPosition.y) * (pBody->mPosition.y - this->mPosition.y);
		
		if (denominator == 0.0f)
			continue;

		double gravScale = numerator / (denominator);
		gravScale /= 1000.0 * 1000.0; // Convert m^2 to km^2

		//std::cout << "DEBUG - Grav scale is " << gravScale << "\n";
		
		// I tried to avoid needing to normalize. I might try again if I have time.

		glm::vec2 gravitationalForce = glm::normalize(pBody->mPosition - this->mPosition) * (float)std::max(gravScale, 10.0) / 1000.0f;

		//std::cout << "DEBUG - grav force: (" << gravitationalForce.x << ", " << gravitationalForce.y << ")" << std::endl;
		// We do not apply gravitational forces that don't make sense here.
		if (
			isnan(gravitationalForce.x)
			|| isnan(gravitationalForce.y)
			|| isinf(gravitationalForce.x)
			|| isinf(gravitationalForce.y)
			)
			return;

		applyForce(gravitationalForce);
	}
}


void PhysicsBody::applyForce(glm::vec2 F)
{
	// Guard against crazy values.
	if (
		isnan(F.x)
		|| isnan(F.y)
		|| isinf(F.x)
		|| isinf(F.y)
		)
		return;

	// Frametime is in ms, convert to s.
	mTotalForce += F;// *GlobalManager::getFrameRateController()->getFrameTimeSec();
}

void PhysicsBody::applyTorque(float T)
{
	// Guard against crazy values.
	if (isnan(T) || isinf(T))
		return;

	mTotalTorque += T;// *GlobalManager::getFrameRateController()->getFrameTimeSec();
}


void PhysicsBody::setTimedIgnoreCollision(PhysicsBody* pIgnored, float ignoreTime)
{
	mIgnorePhysicsBodyTimer = ignoreTime;
	mpIgnoredPhysicsBody = pIgnored;
}


void PhysicsBody::enableGravity()
{
	if (mHasGravity)
		return;

	// Enable gravity and push this physics body onto the physics manager's vector.
	mHasGravity = true;
	GlobalManager::getPhysicsManager()->gravityBodies.push_back(this);
}

void PhysicsBody::disableGravity()
{
	if (!mHasGravity)
		return;
	// Disable gravity and remove this physics body from the physics manager's vector.
	mHasGravity = false;
	GlobalManager::getPhysicsManager()->gravityBodies.remove(this); // TO-DO: A std::list may be more appropriate for gravityBodies... // TO-DO: Will this call this body's destructor by accident???
}


void PhysicsBody::setMass(float mass)
{
	mMass = mass;
	mInvMass = 1 / mass;
}


void PhysicsBody::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::COLLIDE)
	{
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);

		// Only react to your own collisions. Figure out which one you are.
		if (pCollideEvent->mpBodies[0] == this)
		{
			//std::cout << "DEBUG: " << this->mpOwner->mName << " altered\n";
			this->mVelocity = pCollideEvent->mNewVel0;
		}
		else if (pCollideEvent->mpBodies[1] == this)
		{
			//std::cout << "DEBUG: " << this->mpOwner->mName << " altered\n";
			this->mVelocity = pCollideEvent->mNewVel1;
		}
	}
}


void PhysicsBody::setUniformData(ShaderProgram* pProgram)
{
	// TO-DO:
	// 1. Collision type: 0 = none, 1 = aabb, 2 = circle
	auto myShapeType = this->mpShape->mType;

	unsigned int loc;
	if (myShapeType == Shape::ShapeType::CIRCLE)
	{
		loc = glGetUniformLocation(pProgram->ProgramID, "col_type");
		glUniform1i(loc, 1);

		loc = glGetUniformLocation(pProgram->ProgramID, "col_circleRadius");
		//gluniform1f(loc, );
	}
	else if (myShapeType == Shape::ShapeType::AABB)
	{
		loc = glGetUniformLocation(pProgram->ProgramID, "col_type");
		glUniform1i(loc, 2);
	}
	else
	{
		loc = glGetUniformLocation(pProgram->ProgramID, "col_type");
		glUniform1i(loc, 0);
	}

	// 2. if aabb, pass in aabb stuff
	// 3. if circle pass in circle stuff.
	// 4. -- inside the FRAGMENT shader, Apply red to anything within collider.
}


// TO-DO
/*
#include "glm/gtc/type_ptr.hpp"
void PhysicsBody::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{
	// Bind
	glBindVertexArray(vaoID);
	glActiveTexture(GL_TEXTURE0);

	// Uniforms.
	unsigned int loc;
	glm::mat4 myTrans = glm::translate(glm::mat4(1.0f), glm::vec3(this->mPosition, 0.0f) );
	loc = glGetUniformLocation(pProgram->ProgramID, "modelTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(myTrans) );
	loc = glGetUniformLocation(pProgram->ProgramID, "viewTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewTrans));
	loc = glGetUniformLocation(pProgram->ProgramID, "viewProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewProj));

	// Draw.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindVertexArray(0);
}
*/


void PhysicsBody::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check type.
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Error: Physics Body component failed to deserialize. Value was not an object." << std::endl;
		return;
	}

	rapidjson::GenericObject<true, rapidjson::Value> physBodyObj = inputMemberIt->value.GetObject();
	
	// ----- Read in mMass. -----

	if (physBodyObj.HasMember("Mass"))
	{
		if (physBodyObj["Mass"].IsNumber())
		{
			// Store mMass and mInvMass.
			this->setMass(physBodyObj["Mass"].GetFloat());
		}
		else
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Physics Body component's mass parameter was formatted incorrectly. Mass will be default." << std::endl;
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Deserialized Physics Body component did not contain a mass parameter. Mass will be default." << std::endl;

	// ----- Read in shapename. -----

	if (physBodyObj.HasMember("AABB"))
	{
		if (
			physBodyObj["AABB"].IsObject()
			&& physBodyObj["AABB"].HasMember("Left")
			&& physBodyObj["AABB"]["Left"].IsNumber()
			&& physBodyObj["AABB"].HasMember("Right")
			&& physBodyObj["AABB"]["Right"].IsNumber()
			&& physBodyObj["AABB"].HasMember("Top")
			&& physBodyObj["AABB"]["Top"].IsNumber()
			&& physBodyObj["AABB"].HasMember("Bottom")
			&& physBodyObj["AABB"]["Bottom"].IsNumber()
			)
		{
			float left		= physBodyObj["AABB"]["Left"].GetFloat();
			float right		= physBodyObj["AABB"]["Right"].GetFloat();
			float top		= physBodyObj["AABB"]["Top"].GetFloat();
			float bottom	= physBodyObj["AABB"]["Bottom"].GetFloat();

			mpShape = new ShapeAABB(left, right, top, bottom);
			mpShape->mpOwnerBody = this;
		}
		else
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Physics Body component's AABB Shape was improperly formatted." << std::endl;
	}
	else if (physBodyObj.HasMember("Circle"))
	{
		if (
			physBodyObj["Circle"].IsObject()
			&& physBodyObj["Circle"].HasMember("Radius")
			&& physBodyObj["Circle"]["Radius"].IsNumber()
			)
		{
			float radius = physBodyObj["Circle"]["Radius"].GetFloat();

			mpShape = new ShapeCircle(radius);
			mpShape->mpOwnerBody = this;
		}
		else
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Physics Body component's Circle shape was improperly formatted." << std::endl;
	}
	else
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
			std::cout << "Warning: Deserialized Physics Body component did not contain a collision shape parameter." << std::endl;

	// Has velocity?
	if (physBodyObj.HasMember("Velocity"))
	{
		if (physBodyObj["Velocity"].IsArray() && physBodyObj["Velocity"].GetArray().Size() == 2)
		{
			this->mVelocity = glm::vec2(
				physBodyObj["Velocity"].GetArray()[0].GetFloat(),
				physBodyObj["Velocity"].GetArray()[1].GetFloat()
			);
		}
		else
			if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
				std::cout << "Warning: Deserialized Physics Body component's starter velocity was improperly formatted." << std::endl;
	}
	// Has Angular velocity?
	if (physBodyObj.HasMember("Angular Velocity"))
	{
		if (physBodyObj["Angular Velocity"].IsNumber())
		{
			this->mAngularVelocity = physBodyObj["Angular Velocity"].GetFloat();
		}
		else
			std::cout << "Warning: Deserialized Physics Body component's starter angular velocity was improperly formatted." << std::endl;
	}


	// Has gravity?
	if (physBodyObj.HasMember("HasGravity"))
	{
		enableGravity();
	}
}