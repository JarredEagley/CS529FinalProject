#include "EventManager.h"
#include "GlobalManager.h"

EventManager* EventManager::instance = nullptr;

void EventManager::destroySingleton()
{
	delete instance;
}

// End of singleton stuff.
// ------------------------------------------------------------------------ //

void EventManager::broadcastEvent(Event* pEvent)
{
	for (auto pGOPair : GlobalManager::getGameObjectManager()->mGameObjects)
		pGOPair.second->handleEvent(pEvent);
}

void EventManager::addTimedEvent(Event* pEvent)
{
	mEvents.push_back(pEvent);
}

void EventManager::Update(float frameTime)
{
	// Using an iterator because we want to remove from the middle potentially.
	std::list<Event*>::iterator it = mEvents.begin();

	while (it != mEvents.end())
	{
		Event* pEvent = *it;

		pEvent->mTimer -= frameTime;
		if (pEvent->mTimer <= 0.0f)
		{
			broadcastEvent(pEvent);
			delete pEvent;
			it = mEvents.erase(it); // This will update the iterator and remove the element!
		}
		else
			++it;
	}
}