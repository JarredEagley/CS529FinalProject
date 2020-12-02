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
	
	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	float mAngularDamping; // How agressively ship tries to stop rotating with no input.
	glm::vec3 mPlayerAimPoint = glm::vec3(0.0f);

private:
	// No private methods.
private:
	ShipData* mpShipData;
	

};

