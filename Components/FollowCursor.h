#pragma once

#include "Component.h"
#include "Camera.h"

class FollowCursor : public Component
{
public:
	FollowCursor();
	~FollowCursor();

	void Initialize();
	void Update();

	void handleEvent(Event* pEvent);
	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt) {};

public:
	// No public variables.
private:
	// No private methods. 
private:
	Camera* mpCameraComponent;
	Transform* mpCameraTransformComponent;
	Transform* mpTransform;

};

