#include "ShipData.h"

ShipData::ShipData() : Component(ComponentTypes::TYPE_SHIPDATA),
mHealth(100.0f),
mFuel(0.0f), mCoilBullets(0), mMissiles(0),
mMainAcceleration(0.0f), mSecondaryAcceleration(0.0f), mAngularAcceleration(0.0f),
fuelEfficiency(0.0f)
{
}

ShipData::~ShipData()
{
}


void ShipData::Update()
{
}


void ShipData::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}