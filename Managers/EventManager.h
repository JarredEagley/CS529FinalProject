#pragma once
#include <list>

enum EventType
{
	COLLIDE,
	NUM
};

class Event
{
public:
	Event(EventType Type, float timer = 0.0f)
	{
		mType = Type;
		mTimer = timer;
	}
	virtual ~Event() {};

public:
	EventType mType;
	float mTimer;
};

// ------------------------------------ //

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

	void broadcastEvent(Event* pEvent);
	void addTimedEvent(Event* pEvent);
	void Update(float frameTime);
public:
	std::list<Event*> mEvents; // This is a list and not a vector because it's the fastest to remove an element from the middle. Priority queue would also make sense.

	std::unordered_map<EventType, std::list<GameObject*>> mSubscriptions; // unordered_multimap may be appropriate.
private:
	EventManager();
private:
	static EventManager* instance;
};

