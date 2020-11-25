#pragma once

#include "EventManager.h"
class PhysicsBody;

class CollideEvent : public Event // Event declared in EventManager.h
{
public:
	CollideEvent();
	~CollideEvent();
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

