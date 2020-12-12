

#include "OrbitalLine.h"

OrbitalLine::OrbitalLine() : Component(ComponentTypes::TYPE_UNDEFINED)
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
	if (mpLine == nullptr)
	{
		mpLine = static_cast<GLLine*>(mpOwner->GetComponent(ComponentTypes::TYPE_GLLINE));
		return;
	}

	mpLine->mStartPos = glm::vec2(0.0f);
	mpLine->mEndPos = glm::vec2(4000.0f);

}

void OrbitalLine::handleEvent(Event* pEvent)
{
}


void OrbitalLine::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{


}