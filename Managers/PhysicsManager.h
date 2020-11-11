#pragma once


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

public:
	// No public variables.
private:
	PhysicsManager();
private:
	static PhysicsManager* instance;
};

