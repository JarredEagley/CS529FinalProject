#include "GLText.h"

GLText::GLText() : Component(ComponentTypes::TYPE_UNDEFINED)
{

}

GLText::~GLText()
{
}


void GLText::Initialize()
{
}

void GLText::Update()
{
}

//void GLText::handleEvent(Event* pEvent) {};

// Used for components which draw to openGL.
void GLText::setUniformData(ShaderProgram* pProgram)
{}

void GLText::Draw(ShaderProgram* pProgram, glm::mat4 modelTrans, glm::mat4 viewTrans, glm::mat4 viewProj)
{

}


void GLText::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}


