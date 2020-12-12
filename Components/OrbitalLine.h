///
/// DELETE ME IN THE FINAL PRODUCT.
/// 

#pragma once
#include "Component.h"
#include "ComponentTypes.h"
#include "GLLine.h"
#include "PhysicsBody.h"

class OrbitalLine : public Component
{
public:
	OrbitalLine();
	~OrbitalLine();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram) {};
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj) {};

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	int mIterations = 1;
	float mStepSize = 1.0f;

private:
	// No private methods.
private:
	GLLine* mpLine = nullptr;

};

