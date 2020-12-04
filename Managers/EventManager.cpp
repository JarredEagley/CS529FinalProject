/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			EventManager.cpp
	Purpose:			Implements class logic for the EventManager.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/23/2020

- End Header --------------------------------------------------------*/

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

void EventManager::addTimedEvent(Event* pEvent, bool globalEvent)
{
	if (globalEvent)
		pEvent->mIsGlobal = true;
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
			// A bit of a hacky switch between global and sub-based, but it works.
			if (pEvent->mIsGlobal)
				broadcastEvent(pEvent);
			else
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

void EventManager::UnsubscribeAll(GameObject* pGO)
{
	// Loop through each event type.
	for (auto pair : mSubscriptions)
	{
		// Get this event type's list of subscribers. (Need to get by reference!)
		std::list<GameObject*> &listOfSubs = pair.second;

		// Look for this game object inside this list of subscribers.
		auto itr = listOfSubs.begin();
		while (itr != listOfSubs.end())
		{
			// If we've found it, remove it.
			if(*itr == pGO)
			{
				itr = listOfSubs.erase(itr);
			}
			else
				++itr;
		}
		// Super hacky and inefficient, but I think I'm getting copy by value problems.
		mSubscriptions[pair.first] = listOfSubs;
		
	}

}


