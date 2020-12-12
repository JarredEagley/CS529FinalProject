

#include "OrbitalLine.h"

OrbitalLine::OrbitalLine() : Component(ComponentTypes::TYPE_ORBITALLINE)
{
}

OrbitalLine::~OrbitalLine()
{
}


void OrbitalLine::Initialize()
{
}

void OrbitalLine::Update()
{
	if (mpLine == nullptr )
	{
		mpLine = static_cast<GLLine*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLLINE));
		return;
	}
	GameObject* pParentGO = mpOwner->getParent();
	if (pParentGO == nullptr)
		return;
	PhysicsBody* pParentPhsyics = static_cast<PhysicsBody*>(pParentGO->GetComponent(ComponentTypes::TYPE_PHYSICSBODY));
	if (pParentPhsyics == nullptr)
		return;

	mpLine->mStartPos = glm::vec2(0.0f);
	mpLine->mEndPos = pParentPhsyics->mVelocity;

}

void OrbitalLine::handleEvent(Event* pEvent)
{
}


void OrbitalLine::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{


}