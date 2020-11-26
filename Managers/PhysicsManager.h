#pragma once

#include "EventManager.h"
#include "glm/vec2.hpp"

class PhysicsBody;

class CollideEvent : public Event // Event declared in EventManager.h
{
public:
	CollideEvent();
	~CollideEvent();

	glm::vec2 mTotalVelocity;
	float mTotalSpeed;
	glm::vec2 mVelocityNormalized;
	glm::vec2 mNormal;
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

	void Update();

public:
	static std::list<PhysicsBody*> gravityBodies;
private:
	PhysicsManager();
private:
	static PhysicsManager* instance;
};

