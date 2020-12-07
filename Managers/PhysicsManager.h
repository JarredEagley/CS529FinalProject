#pragma once

#include "EventManager.h"
#include "glm/vec2.hpp"

class PhysicsBody;

class CollideEvent : public Event // Event declared in EventManager.h
{
public:
	CollideEvent();
	~CollideEvent();

	PhysicsBody* mpBodies[2];

	glm::vec2 mNewVel0;
	glm::vec2 mNewVel1;

	bool mObjectsAreApproaching;
};


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

private:
	PhysicsManager();
private:
	static PhysicsManager* instance;
};

