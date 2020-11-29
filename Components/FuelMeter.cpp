#include "FuelMeter.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"
#include "ShipData.h"

FuelMeter::FuelMeter() : Component(ComponentTypes::TYPE_METER_FUEL),
mFuel(0.0f), mSecondaryColor(glm::vec4(1.0f))
{}

FuelMeter::~FuelMeter()
{}


void FuelMeter::Initialize()
{
	// We want to know when shipdata updates.
	GlobalManager::getEventManager()->Subscribe(EventType::SHIPDATA_UPDATED, mpOwner);
}

void FuelMeter::Update() 
{
}

void FuelMeter::setUniformData(ShaderProgram* pProgram)
{
	//std::cout << "sending fuel data: " << mFuel << std::endl;
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "fillLevel");
	glUniform1f(loc, mFuel);
	loc = glGetUniformLocation(pProgram->ProgramID, "secondaryColor");
	glUniform4fv(loc, 1, &mSecondaryColor.x);

}


void FuelMeter::handleEvent(Event* pEvent)
{
	// Listen for shipdata updates.
	if (pEvent->mType == EventType::SHIPDATA_UPDATED)
	{
		ShipDataUpdatedEvent* pShipDataEvent = static_cast<ShipDataUpdatedEvent*>(pEvent);
		this->mFuel = pShipDataEvent->mpShipData->mFuel;
	}
}


void FuelMeter::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	rapidjson::GenericObject<true, rapidjson::Value> inputObj = inputMemberIt->value.GetObject();

	// Try to deserialize color.
	if (inputObj.HasMember("Secondary Color"))
	{
		if (inputObj["Secondary Color"].IsArray())
		{
			// Get the array from the JSON...
			rapidjson::GenericArray<true, rapidjson::Value> colorArray = inputObj["Secondary Color"].GetArray();

			// Check array formatting.
			if (colorArray.Capacity() == 4 && colorArray[0].IsNumber())
			{
				// Formatting is ok. Set color.
				glm::vec4 ourcolor = glm::vec4(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat(), colorArray[3].GetFloat());
				this->mSecondaryColor = ourcolor;
			}
			else
			{
				std::cout << "Warning: GLRect had a 'Color' but its array was incorrectly formatted!" << std::endl;
			}
		}
		else
		{
			std::cout << "Warning: GLRect had a 'Color' but its value was not an array of numbers!" << std::endl;
		}
	}

}

