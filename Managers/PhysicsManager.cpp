#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = nullptr;

void PhysicsManager::destroySingleton()
{
	delete instance;
}
// End of singleton stuff //

PhysicsManager::PhysicsManager()
{
}


