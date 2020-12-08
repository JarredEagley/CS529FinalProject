#pragma once
#include "Component.h"
#include "ComponentTypes.h"

#include "Transform.h"
#include "PhysicsBody.h"


class Explosion : public Component
{
public:
	Explosion();
	~Explosion();

	void Initialize();
	void Update(); 

	void setUniformData(ShaderProgram* pProgram);

	void handleEvent(Event* pEvent);

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	float mIntensity = 100.0f;
	float mSize = 0.1f;

private:
	// No private methods.
private:
	Transform* mpTransform;
	PhysicsBody* mpPhysicsBody;
};

