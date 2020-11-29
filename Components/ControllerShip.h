#pragma once

#include "Component.h"
#include "rapidjson/document.h"
#include "ShipData.h"

class ControllerShip : public Component
{
public:
	ControllerShip();
	//ControllerShip(float speed);
	~ControllerShip();

	void Update();
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	float mAngularDamping; // How agressively ship tries to stop rotating with no input.

private:
	// No private methods.
private:
	ShipData* mpShipData;
	

};

