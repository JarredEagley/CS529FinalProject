#pragma once
#include <list>
#include <unordered_map>

class GameObject;

enum class EventType
{
	COLLIDE,
	TRANSFORM_UPDATED,
	PHYSICS_BODY_UPDATED,

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

// I couldn't think of a better place for this.
class Transform;
class TransformUpdatedEvent : public Event
{
public:
	TransformUpdatedEvent(Transform* ptransform) : Event(EventType::TRANSFORM_UPDATED), mpTransform(ptransform)
	{}

	~TransformUpdatedEvent() {}

	Transform* mpTransform;
};

/*
class PhysicsBody;
class PhysicsUpdatedEvent : public Event
{
public:
	PhysicsUpdatedEvent(PhysicsBody* pPBody) : Event(EventType::PHYSICS_BODY_UPDATED), mpPhysicsBody(pPBody)
	{}

	~PhysicsUpdatedEvent() {}

	PhysicsBody* mpPhysicsBody;

};
*/

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

	void broadcastEvent(Event* pEvent); // Broadcast to everybody.
	void broadcastEventToSubscribers(Event* pEvent);
	void addTimedEvent(Event* pEvent);
	void Update();

	void Subscribe(EventType Et, GameObject* pGameObject); 
	 
public:
	static std::list<Event*> mEvents; // This is a list and not a vector because it's the fastest to remove an element from the middle. Priority queue would also make sense.

	static std::unordered_map<EventType, std::list<GameObject*>> mSubscriptions; // unordered_multimap may be appropriate.

private:
	EventManager();
private:
	static EventManager* instance;
};

