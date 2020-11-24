#pragma once

#include "EventManager.h"
class PhysicsBody;

class CollideEvent : public Event
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

	void Update(float frameTime);

public:
	static std::list<PhysicsBody*> gravityBodies;
private:
	PhysicsManager();
private:
	static PhysicsManager* instance;
};

