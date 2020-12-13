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

	if (mpAICore->isInGravity)
	{
		float playerdist = mpAICore->mPlayer_Distance;

		// Dangerous, but I'll never have a non-circle gravity body.
		Shape * pNGBShape = mpAICore->mpNearestGravityBody->mpShape;
		ShapeCircle* pNGMShapeCircle = static_cast<ShapeCircle*>(pNGBShape);
			
		// A little bit big. A warning rather than a hard limit.
		float minimumOrbitalRadius = pNGMShapeCircle->mRadius*1.2f + 100.0f;

		if (playerdist > bruteForceDistance)
		{

			// Will be negative if player is in front, positive if player is behind.
			float playerAltitude = glm::length(mpAICore->mPlayerPosition - mpAICore->mpNearestGravityBody->mPosition);
			float desiredAltitudeRelativeToPlayer = -glm::dot(mpAICore->mPlayer_RelativePositionNormal, glm::normalize(mpAICore->mNGB_RelativePosition));

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
				if (playerdist > minimumDistance)
				{
					// Chase
					mpAICore->matchVelocityVector(mpAICore->mPlayerVelocity + mpAICore->mPlayer_RelativePosition);
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
		//mpAICore->tryToStop();










	}
}


void AIEnemyChase::handleEvent(Event* pEvent)
{
}


void AIEnemyChase::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Nothing to deserialize.
}