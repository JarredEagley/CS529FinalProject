/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			EventManager.h
	Purpose:			The event manager manages events, which are the main
						way game logic is communicated.
						Other than collision events which are defined
						in the collision manager, all events are defined
						here.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/23/2020

- End Header --------------------------------------------------------*/

#pragma once

#include <list>
#include "Events.h" // Contains EventTypes and a bunch of event definitions.
#include <unordered_map>
#include "glm/glm.hpp"

class GameObject;

class EventManager
{
public:
	static EventManager* getInstance()
	{
		if (!instance)
			instance = new EventManager;
		return instance;
	};
	void destroySingleton();

	void broadcastEvent(Event* pEvent); // Broadcast to everybody.
	void broadcastEventToSubscribers(Event* pEvent);
	void addTimedEvent(Event* pEvent, bool globalEvent = false); // Note: Event will only be broadcasted to subscribers.
	void Update();

	void Subscribe(EventType Et, GameObject* pGameObject); 
	//void Unsubscribe(EventType Et, GameObject* pGO);
	void UnsubscribeAll(GameObject *pGO);
	 
public:
	static std::list<Event*> mEvents; // This is a list and not a vector because it's the fastest to remove an element from the middle. Priority queue would also make sense.
	static std::unordered_map<EventType, std::list<GameObject*>> mSubscriptions; // unordered_multimap may be appropriate.

private:
	EventManager();
private:
	static EventManager* instance;
};

