#include "Explosion.h"
#include "ComponentTypes.h"
#include "../Managers/GlobalManager.h"

Explosion::Explosion() : Component(ComponentTypes::TYPE_EXPLOSION)
{
}

Explosion::~Explosion()
{

}


void Explosion::Initialize()
{

}

void Explosion::Update()
{
	// Explosion always starts at 0.1 size.
	// Input is the intensity.
	// Rate of change is same for size and intensity.
	// For now, fixed rate of change. If I need to change this later, I will.
	if (mIntensity > 0)
	{
		float deltaTime = GlobalManager::getPhysicsManager()->getGameTime();
		// Decrement intensity.
		mIntensity -= deltaTime;
		// Increment size.
		mSize += deltaTime;
	}
	else
	{
		mpOwner->mIsMarkedForDelete = true;
	}
}


void Explosion::setUniformData(ShaderProgram* pProgram)
{}


void Explosion::handleEvent(Event* pEvent)
{
	if (pEvent->mType == EventType::COLLIDE)
	{
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);

		// TO-DO:
		// Our physics body's shape needs to grow in size
		
		// Figure out how far away "colliding" object is
		// Deal damage to it based on distance and intensity.
	}
}

void Explosion::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) 
{
	auto inputObj = inputMemberIt->value.GetObject();

	if (inputObj.HasMember("Intensity") && inputObj["Intensity"].IsNumber())
	{
		this->mIntensity = inputObj["Intensity"].GetFloat();
	}
	else
	{
		if (GlobalManager::getGameStateManager()->DEBUG_VerboseComponents)
		{
			std::cout << "Error: Explosion component failed to deserialize; Intensity was missing or incorrectly formatted. This may create an orphan game object!" << std::endl;
		}
	}
}
