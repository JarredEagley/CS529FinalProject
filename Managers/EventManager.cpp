#include "EventManager.h"
#include "GlobalManager.h"

EventManager* EventManager::instance = nullptr;
std::list<Event*> EventManager::mEvents;
std::unordered_map<EventType, std::list<GameObject*>> EventManager::mSubscriptions;


void EventManager::destroySingleton()
{
	// Clear the subscriptions map. (Don't delete GameObjects by accident!)
	for (auto li : mSubscriptions)
		li.second.clear();
	mSubscriptions.clear();

	// Clear the events list.
	for (auto pEv : mEvents)
		delete pEv;
	mEvents.clear();

	delete instance;
}

// End of singleton stuff.
// ------------------------------------------------------------------------ //

EventManager::EventManager()
{
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
	// Loop through the event list.
	std::list<Event*>::iterator it = mEvents.begin();

	while (it != mEvents.end())
	{
		// Get current event.
		Event* pEvent = *it;

		pEvent->mTimer -= GlobalManager::getFrameRateController()->getFrameTime();
		if (pEvent->mTimer <= 0.0f)
		{
			//broadcastEvent(pEvent);
			// Broadcast that event to its subscribers then remove it.
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
	// Get all subscribed to this event type.
	std::list<GameObject*>& listOfSubs = mSubscriptions[Et];

	// Don't double-add.
	for (auto pGO : listOfSubs) 
		if (pGO == pGameObject)
			return;

	// Push onto that list.
	listOfSubs.push_back(pGameObject);
}
