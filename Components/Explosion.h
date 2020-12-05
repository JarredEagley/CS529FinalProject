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
private:
private:
};

