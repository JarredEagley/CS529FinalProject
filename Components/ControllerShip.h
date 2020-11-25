#pragma once

#include "Component.h"
#include "rapidjson/document.h"

class ControllerShip : public Component
{
public:
	ControllerShip();
	ControllerShip(float speed);
	~ControllerShip();

	void Update();
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	// No public variables.
private:
	// No private methods.
private:
	float mThrottle; // Throttle
	float mMainAcceleration; // Main drive
	float mSecondaryAcceleration; // Maneuvering thrusters
	float mAngularAcceleration; // RCS

};

