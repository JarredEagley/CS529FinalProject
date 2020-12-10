///
/// DELETE ME IN THE FINAL PRODUCT.
/// 

#pragma once
#include "Component.h"
#include "ComponentTypes.h"

class AIEnemyStationary : public Component
{
public:
	AIEnemyStationary();
	~AIEnemyStationary();

	void Initialize();
	void Update();
	void handleEvent(Event* pEvent);

	// Used for components which draw to openGL.
	void setUniformData(ShaderProgram* pProgram) {};
	void Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj) {};

	void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
private:
	// No private methods.
private:
	// No private variables.
};

