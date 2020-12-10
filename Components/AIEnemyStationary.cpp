

#include "AIEnemyStationary.h"

AIEnemyStationary::AIEnemyStationary() : Component(ComponentTypes::TYPE_UNDEFINED)
{
	// Register with the enemies list.
}

AIEnemyStationary::~AIEnemyStationary()
{
	// Remove from the enemies list.
}


void AIEnemyStationary::Initialize()
{
}

void AIEnemyStationary::Update()
{
}

void AIEnemyStationary::handleEvent(Event* pEvent)
{
}


void AIEnemyStationary::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{



}