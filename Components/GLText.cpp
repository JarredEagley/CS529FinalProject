#include "GLText.h"
#include "../Managers/GlobalManager.h"

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
	// Things to move soon:
	float x, y, scale;
	glm::vec3 color;

	// Iterate through the string.
	for (std::string::const_iterator c = mTextContent.begin(); c != mTextContent.end(); ++c)
	{
		Character ch = GlobalManager::getResourceManager()->mCharacters[*c];

		float xPos = x + ch.mBearing.x * scale;
		float yPos = y - (ch.mSize.y - ch.mBearing.y) * scale;

		float w = ch.mSize.x * scale;
		float h = ch.mSize.y * scale;

		// Update the VBO for each character...
		// bunch of crap in here needs to be done.

		// Render glyph texture over this quad.
		glBindTexture(GL_TEXTURE_2D, ch.mTexId);
		// Update content of VBO Memory.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Need the vbo to be a thing.
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // More things to do...
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Draw call.
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance cursor to next glyph.
		x += (ch.mAdvance >> 6) * scale;
	}

	// Unbind.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D);
}

void GLText::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{

}


