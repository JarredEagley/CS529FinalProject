/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			PhysicsManager.h
	Purpose:			The physics manager keeps track of gravity,
						values regarding epxlosions and damage, game time,
						and resolves collisions from the collision manager.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/10/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "EventManager.h"
#include "glm/glm.hpp"

class PhysicsBody;

class PhysicsManager
{
public:
	static PhysicsManager* getInstance()
	{
		if (!instance)
			instance = new PhysicsManager;
		return instance;
	};
	void destroySingleton();

	float getGameTime(); // In game seconds per real life seconds, multiplier by the game time multiplier.

	// Important note: Result must be multiplied by object mass before force is applied!
	glm::vec2 calculateGravitationalForces(glm::vec2 position);

	void Update();

public:
	static std::list<PhysicsBody*> gravityBodies;
	//static bool isPhysicsPaused;
	static float gameTimeMultiplier; // At realistic scale these battles would take place over days. This is a multiplier for the flow of time.
	static float piercingThreshold;
	static float piercingDamageMultiplier;
	static float deflectDamageMultiplier;
	static float explosionDamageMultiplier;

	static float explosionExpansionRate;
	static float explosionDecayRate;

	static float universalGravitationalConstant;

private:
	PhysicsManager();
private:
	static PhysicsManager* instance;

};

