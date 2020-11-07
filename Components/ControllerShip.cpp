#include "ControllerShip.h"

#include "ComponentTypes.h"

ControllerShip::ControllerShip() : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
{

}

ControllerShip::ControllerShip(float speed) : Component(ComponentTypes::TYPE_CONTROLLERSHIP)
{

}

ControllerShip::~ControllerShip()
{

}

void ControllerShip::Update()
{

}

void ControllerShip::setSpeed(float speed)
{

}
	
void ControllerShip::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}
