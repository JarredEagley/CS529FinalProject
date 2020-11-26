#pragma once

#include "Component.h"
#include "rapidjson/document.h"
#include "ShipData.h"
#include "PhysicsBody.h"

class ControllerShip : public Component
{
public:
	ControllerShip();
	//ControllerShip(float speed);
	~ControllerShip();

	void Update();
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	float mThrottle; // Throttle
	float mThrottleSensitivity;

	ShipData* mpShipData;
	PhysicsBody* mpPhysicsBody;

};

