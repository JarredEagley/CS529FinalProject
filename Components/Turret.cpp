#include "Turret.h"

#include "ComponentTypes.h"

Turret::Turret() : Component(ComponentTypes::TYPE_TURRET)
{

}

Turret::~Turret()
{

}


void Turret::Initialize()
{

}

void Turret::Update()
{

}



void Turret::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}

void Turret::handleEvent(Event* pEvent) {};

