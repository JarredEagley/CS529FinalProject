#pragma once
#include <list>
#include <unordered_map>
#include "glm/glm.hpp"

class GameObject;

enum class EventType
{
	COLLIDE,
	TRANSFORM_UPDATED,
	SHIPDATA_UPDATED,
	CAMERA_TRANSFORM_UPDATED,
	MOUSE_SCROLL,
	TURRET_COMMAND,
	DESTROY_PROJETILE,
	CREATE_PROJECTILE,

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
	bool mIsGlobal = false;

public:
	EventType mType;
	float mTimer;
};

// I couldn't think of a better place for these.
class Transform;
class TransformUpdatedEvent : public Event
{
public:
	TransformUpdatedEvent(Transform* ptransform) : Event(EventType::TRANSFORM_UPDATED), mpTransform(ptransform)
	{}

	~TransformUpdatedEvent() {}

	Transform* mpTransform;
};

class ShipData;
class ShipDataUpdatedEvent : public Event
{
public:
	ShipDataUpdatedEvent(ShipData* pShipData) : Event(EventType::SHIPDATA_UPDATED), mpShipData(pShipData)
	{}

	~ShipDataUpdatedEvent() {};

	ShipData* mpShipData;
};

class CameraTransformUpdatedEvent : public Event
{
public:
	CameraTransformUpdatedEvent() : Event(EventType::CAMERA_TRANSFORM_UPDATED) {};
	~CameraTransformUpdatedEvent() {};
};

class MouseScrollEvent : public Event
{
public:
	MouseScrollEvent() : Event(EventType::MOUSE_SCROLL) {};
	~MouseScrollEvent() {};
};

class TurretCommandEvent : public Event
{
public:
	TurretCommandEvent() : Event(EventType::TURRET_COMMAND) {}
	~TurretCommandEvent() {}

	bool mShoot;
	glm::vec2 mAimPoint;
};

// For example purposes, this is a timed event.
class DestroyAllProjectilesEvent : public Event
{
public:
	DestroyAllProjectilesEvent() : Event(EventType::DESTROY_PROJETILE) {};
	~DestroyAllProjectilesEvent() {};
};

class CreateProjectileEvent : public Event
{
public:
	// Holds onto the game object firing the projectile
	CreateProjectileEvent(GameObject* pGO) : Event(EventType::CREATE_PROJECTILE), mpGO(pGO) {};
	~CreateProjectileEvent() {};

	GameObject* mpGO;
	// May eventually also want to hold a projectile type, but I'm not doing that right now.
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

	void broadcastEvent(Event* pEvent); // Broadcast to everybody.
	void broadcastEventToSubscribers(Event* pEvent);
	void addTimedEvent(Event* pEvent, bool globalEvent = false); // Note: Event will only be broadcasted to subscribers.
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

