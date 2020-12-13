/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			AIEnemyChase.cpp
	Purpose:			Implementations for AIEnemyChase.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/10/2020

- End Header --------------------------------------------------------*/

#include "AIEnemyChase.h"
#include "../Managers/GlobalManager.h"
#include "glm/gtx/projection.hpp"


AIEnemyChase::AIEnemyChase() : Component(ComponentTypes::TYPE_AI_CHASE)
{
}

AIEnemyChase::~AIEnemyChase()
{
}


void AIEnemyChase::Initialize()
{
}


void AIEnemyChase::Update()
{
	if (GlobalManager::getGameStateManager()->mIsGamePaused)
		return;

	if (mpAICore == nullptr)
	{
		mpAICore = static_cast<AIEnemyCore*>(mpOwner->GetComponent(ComponentTypes::TYPE_AI_CORE));
	}

	// --- behavior --- //

	float playerdist = mpAICore->mPlayer_Distance;

	if (mpAICore->isInGravity)
	{

		// Dangerous, but I'll never have a non-circle gravity body.
		Shape * pNGBShape = mpAICore->mpNearestGravityBody->mpShape;
		if (pNGBShape == nullptr)
			return;
		ShapeCircle* pNGMShapeCircle = static_cast<ShapeCircle*>(pNGBShape);
		if (pNGMShapeCircle == nullptr)
			return;
			
		
		// A little bit big. A warning rather than a hard limit.
		float minimumOrbitalRadius = pNGMShapeCircle->mRadius*1.2f + 100.0f;

		if (playerdist > bruteForceDistance)
		{

			// Will be negative if player is in front, positive if player is behind.
			float playerAltitude = glm::length(mpAICore->mPlayerPosition - mpAICore->mpNearestGravityBody->mPosition);
			float desiredAltitudeRelativeToPlayer = -glm::dot(mpAICore->mPlayer_RelativePositionNormal, glm::normalize(mpAICore->mNGB_RelativePosition));

			if (mInvertedBehavior)
				desiredAltitudeRelativeToPlayer *= -1;

			float desiredaltitude = playerAltitude + desiredAltitudeRelativeToPlayer;

			mpAICore->mDesiredAltitude = std::max(desiredaltitude, minimumOrbitalRadius);
			mpAICore->keepOrbit();
		}
		else
		{
			// Safety check.
			if (mpAICore->mNGB_Dist <= minimumOrbitalRadius)
			{
				mpAICore->mDesiredAltitude = minimumOrbitalRadius;
				mpAICore->keepOrbit();
			}
			else
			{
				// Its safe, Do brute force.
				if (playerdist > minimumDistance || mInvertedBehavior)
				{
					// Chase
					glm::vec2 chaseVec = glm::normalize(mpAICore->mPlayerVelocity - mpAICore->mPlayer_RelativePosition) * mChaseAgression;

					if (mInvertedBehavior)
						chaseVec *= -1;

					mpAICore->matchVelocityVector(chaseVec);
				}
				else
				{
					// Match
					mpAICore->matchVelocityVector(mpAICore->mPlayerVelocity);
				}
			}
		}
	}
	else
	{
		// Go toward player.
		if (playerdist > minimumDistance || mInvertedBehavior)
		{
			glm::vec2 chaseVec = glm::normalize(mpAICore->mPlayerVelocity - mpAICore->mPlayer_RelativePosition) * mChaseAgression;

			if (mInvertedBehavior)
				chaseVec *= -1;

			mpAICore->matchVelocityVector(chaseVec);
		}
		else
		{
			mpAICore->matchVelocityVector(mpAICore->mPlayerVelocity);
		}
	}
}


void AIEnemyChase::handleEvent(Event* pEvent)
{
}


void AIEnemyChase::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Bruteforce Distance") && inputObj["Bruteforce Distance"].IsNumber())
		this->bruteForceDistance = inputObj["Bruteforce Distance"].GetFloat();

	if (inputObj.HasMember("Minimum Distance") && inputObj["Minimum Distance"].IsNumber())
		this->bruteForceDistance = inputObj["Minimum Distance"].GetFloat();

	if (inputObj.HasMember("Chase Agression") && inputObj["Chase Agression"].IsNumber())
		this->mChaseAgression = inputObj["Chase Agression"].GetFloat();



	// Makes the AI run away from the player if they get too close.
	if (inputObj.HasMember("Inverted Behavior"))
		this->mInvertedBehavior = true;
}