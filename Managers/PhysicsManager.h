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

	static float universalGravitationalConstant;

private:
	PhysicsManager();
private:
	static PhysicsManager* instance;

};

