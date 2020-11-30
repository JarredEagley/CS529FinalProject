#pragma once

#include "Component.h"
#include "../Managers/GlobalManager.h"

class Transform;
class GLRect;

class Turret : public Component
{
public:
	Turret();
	~Turret();

	void Initialize();
	void Update(); 

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
	void handleEvent(Event* pEvent);

public:
	glm::vec2 mAimPoint;
private:
	float mAimAngle;
private:
	int fireRate = 60;
	int fireTimer = 40;
	bool mIsShooting;
	Transform* mpTransform;
	Transform* mpParentTransform;
	GLRect* mpGLRect;
	GLRect* mpParentGLRect;

};

