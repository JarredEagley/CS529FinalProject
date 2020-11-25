#include "EventManager.h"
#include "GlobalManager.h"

EventManager* EventManager::instance = nullptr;
std::list<Event*> EventManager::mEvents;
std::unordered_map<EventType, std::list<GameObject*>> EventManager::mSubscriptions;


void EventManager::destroySingleton()
{
	delete instance;
}

// End of singleton stuff.
// ------------------------------------------------------------------------ //

EventManager::EventManager()
{
	// Empty for now.
}

void EventManager::broadcastEvent(Event* pEvent)
{
	for (auto pGOPair : GlobalManager::getGameObjectManager()->mGameObjects)
		pGOPair.second->handleEvent(pEvent);
}

void EventManager::addTimedEvent(Event* pEvent)
{
	mEvents.push_back(pEvent);
}

void EventManager::Update()
{
	// Using an iterator because we want to remove from the middle potentially.
	std::list<Event*>::iterator it = mEvents.begin();

	while (it != mEvents.end())
	{
		Event* pEvent = *it;

		pEvent->mTimer -= GlobalManager::getFrameRateController()->getFrameTime();
		if (pEvent->mTimer <= 0.0f)
		{
			//broadcastEvent(pEvent);
			broadcastEventToSubscribers(pEvent);
			delete pEvent;
			it = mEvents.erase(it); // This will update the iterator and remove the element!
		}
		else
			++it;
	}
}


void EventManager::broadcastEventToSubscribers(Event* pEvent)
{
	std::list<GameObject*>& listOfSubs = mSubscriptions[pEvent->mType];

	for (auto pGO : listOfSubs)
		pGO->handleEvent(pEvent);
}

void EventManager::Subscribe(EventType Et, GameObject* pGameObject)
{
	std::list<GameObject*>& listOfSubs = mSubscriptions[Et];

	for (auto pGOPair : GlobalManager::getGameObjectManager()->mGameObjects) // TO-DO: Am I 100% sure this is right??
		if (pGOPair.second == pGameObject)
			return;

	listOfSubs.push_back(pGameObject);
}
