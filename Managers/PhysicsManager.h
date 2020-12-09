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

	void Update();

public:
	static std::list<PhysicsBody*> gravityBodies;
	static bool isPhysicsPaused;
	static float gameTimeMultiplier; // At realistic scale these battles would take place over days. This is a multiplier for the flow of time.
	static float piercingThreshold;
	static float piercingDamageMultiplier;
	static float deflectDamageMultiplier;
	static float explosionDamageMultiplier;

private:
	PhysicsManager();
private:
	static PhysicsManager* instance;

};

