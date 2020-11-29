#pragma once

#include "Component.h"

class Turret : public Component
{
public:
	Turret();
	~Turret();

	void Initialize();
	void Update(); 

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
	void handleEvent(Event* pEvent);
public:
private:
private:
};

