/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Events.h
	Purpose:			EventType enum class and event definitions from
						EventManager's header file, extracted out for
						cleanliness.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/6/20

- End Header --------------------------------------------------------*/

#include "glm/glm.hpp"
#include <string>

class GameObject;
class PhysicsBody;

enum class EventType
{
	COLLIDE,
	GAMEOBJECT_DESTROYED,
	PLAYER_KILLED,
	PLAYER_OUT_OF_BOUNDS,

	TRANSFORM_UPDATED,
	TRANSFORM_SETPOSITION,
	CAMERA_TRANSFORM_UPDATED,

	SHIPDATA_UPDATED,
	MOUSE_SCROLL,
	CURSOR_WORLD_COORDS,
	TURRET_COMMAND,
	MISSILE_LOCKON,
	MENU_ITEM_CLICKED,
	
	DESTROY_PROJETILE,
	CREATE_PROJECTILE,
	DO_DAMAGE,

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

// ----- //

// Altered from the original example; now we instead send
// two different collide events to the two different bodies
// so that they may behave accordingly.
class CollideEvent : public Event 
{
public:
	CollideEvent();
	~CollideEvent();

	enum collisionResponse
	{
		DEFLECT, // Bounce
		PIERCE,  // Pass, but slow down and do damage.
		PASS	 // No collision
	};

	// Potentially used by all
	PhysicsBody * mpOtherBody;
	collisionResponse mResponse;
	bool mObjectsAreApproaching = false;
	float mRelativeSpeed;

	//glm::vec2 mCollisionNormal;

	// Elastic collision
	glm::vec2 mNewVelocity; // Specifically for bounce.

	// Piercing collision
	glm::vec2 mRelativeVelocity;

	// Used by explosions.
	float mDistance;

};

class PlayerKilledEvent : public Event
{
public:
	PlayerKilledEvent() : Event(EventType::PLAYER_KILLED) {};
	~PlayerKilledEvent() {};
};

class PlayerOutOfBoundsEvent : public Event
{
public:
	PlayerOutOfBoundsEvent(float distance) : Event(EventType::PLAYER_OUT_OF_BOUNDS), mDistance(distance) {};
	~PlayerOutOfBoundsEvent() {};

	float mDistance;
};



class Transform;
class TransformUpdatedEvent : public Event
{
public:
	TransformUpdatedEvent(Transform* ptransform) : Event(EventType::TRANSFORM_UPDATED), mpTransform(ptransform)
	{}

	~TransformUpdatedEvent() {}

	Transform* mpTransform;
};

// Used to set the position of another game object.
class SetTransformPositionEvent : public Event
{
public:
	SetTransformPositionEvent(glm::vec2 pos) : Event(EventType::TRANSFORM_SETPOSITION), mPos(pos)  {};
	~SetTransformPositionEvent() {};
	glm::vec2 mPos;
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

// When missiles lock onto a target, they inform that target.
// Carry the name of the missile game object so AI's can find the game object and shoot it.
class MissileLockOnEvent : public Event
{
public:
	MissileLockOnEvent(std::string missileGOName) : Event(EventType::MISSILE_LOCKON), mMissileGOName(missileGOName) {};
	~MissileLockOnEvent() {};

	std::string mMissileGOName;
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


class DoDamageEvent : public Event
{
public:
	DoDamageEvent(float damage) : Event(EventType::DO_DAMAGE), mDamage(damage) {};
	~DoDamageEvent() {};
	float mDamage;
};


class MenuItemClickedEvent : public Event
{
public:
	MenuItemClickedEvent(std::string command) : Event(EventType::MENU_ITEM_CLICKED), mCommand(command) {};
	~MenuItemClickedEvent() {};

	std::string mCommand;
};