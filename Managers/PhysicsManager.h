#pragma once

#include "EventManager.h"

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
	// No public variables.
private:
	PhysicsManager();
private:
	static PhysicsManager* instance;
};

