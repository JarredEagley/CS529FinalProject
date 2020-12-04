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
#include <unordered_map>
#include "glm/glm.hpp"

class GameObject;

enum class EventType
{
	COLLIDE,
	GAMEOBJECT_DESTROYED,
	TRANSFORM_UPDATED,
	SHIPDATA_UPDATED,
	CAMERA_TRANSFORM_UPDATED,
	MOUSE_SCROLL,
	TURRET_COMMAND,
	DESTROY_PROJETILE,
	CREATE_PROJECTILE,
	EXAMPLE_SPIN,
	CURSOR_WORLD_COORDS,

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
	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;
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

class CursorToWorldCoordinatesEvent : public Event
{
public:
	CursorToWorldCoordinatesEvent() : Event(EventType::CURSOR_WORLD_COORDS) {};
	~CursorToWorldCoordinatesEvent() {};
	glm::vec3 mCoords;
};


class GameObjectDestroyedEvent : public Event
{
public:
	GameObjectDestroyedEvent(std::string destroyedGOName) : Event(EventType::GAMEOBJECT_DESTROYED), mDestroyedGOName(destroyedGOName) {};
	~GameObjectDestroyedEvent() {};
	std::string mDestroyedGOName;
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

