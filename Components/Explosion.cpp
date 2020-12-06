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
	if (mIntensity > 0)
	{
		// 



		// Decrement intensity.
		mIntensity -= GlobalManager::getFrameRateController()->getFrameTime();
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

}

void Explosion::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) 
{

}
