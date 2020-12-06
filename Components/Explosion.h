#pragma once
#include "Component.h"

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
	float mSize = 1.0f;

private:
	// No private methods.
private:
	// No private variables.
};

