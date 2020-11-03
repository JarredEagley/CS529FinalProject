#include "GLRect.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"
#include <iostream>

GLRect::GLRect() : Component(ComponentTypes::TYPE_GLRECT)
{
	// Texture is null until loaded.
	mTexture = nullptr;
	vaoID = NULL;

	// Vertex positions for a quad.
	vertices[0].position = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);	// Top left
	vertices[1].position = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);	// Bottom left
	vertices[2].position = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);	// Bottom right
	vertices[3].position = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);	// Top right
	// Vertex UV's for a quad.
	vertices[0].uv = glm::vec2(0.0f, 1.0f); 	// Top left
	vertices[1].uv = glm::vec2(0.0f, 0.0f); 	// Bottom left
	vertices[2].uv = glm::vec2(1.0f, 0.0f); 	// Bottom right
	vertices[3].uv = glm::vec2(1.0f, 1.0f); 	// Top right
	// Set all the colors to white opaque.
	for (auto vert : vertices)
		vert.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

GLRect::~GLRect()
{
	// TO-DO: Will I need to destroy a VAO here?
}

void GLRect::Update()
{

}

/* Old monolithic way of doing this...
void GLRect::setColor(float r, float g, float b, float a)
{
	// For each vert.
	for (int i = 0; i < 4; ++i)
	{
		const int rowsize = 9;
		mVertices[3 + (i * rowsize)] = r;
		mVertices[4 + (i * rowsize)] = g;
		mVertices[5 + (i * rowsize)] = b;
		mVertices[6 + (i * rowsize)] = a;
	}
}
*/


void GLRect::setColor(glm::vec4 rgba)
{
	for (auto vert : vertices)
		vert.color = rgba;
}

// Draws using the openGL vertex array object.
void GLRect::Draw(Shader* shader)
{

}

// Builds the openGL vertex array object. 
void GLRect::buildVAO()
{
	// VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 *); //?

	// EBO

	// Enable the vertex attribute.
	glEnableVertexAttribArray(0);

	//glVertexAttribIPointer(0, x, GL_FLOAT, GL_FALSE, 0); // 0 poitner? weird.
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Is this binding that previous 0?

	// Bunch of if statements

	// 

	GLuint indexBuff;
	glGenBuffers(1, &indexBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuff);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * ); //??? 

	glBindVertexArray(0); // Set back to default.
}

void GLRect::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check for serializing GLRect.
	if (!inputMemberIt->value.IsObject())
	{
		std::cerr << "Warning: GLRect failed to deserialize. Value was not formatted correctly.." << std::endl;
		return;
	}
	// Get the GLRect object.
	rapidjson::GenericObject<true, rapidjson::Value> rectObject = inputMemberIt->value.GetObject();

	char* storedTexture = nullptr; // TO-DO: Do I need these? Probably not...
	int storedColor[4]; // RGBA

	// Try to deserialize texture.
	if (rectObject.HasMember("Texture"))
	{
		if (rectObject["Texture"].IsString())
		{
			// Read in that texture.
			const std::string imageName = rectObject["Texture"].GetString();
			const std::string imagePath = GlobalManager::getResourceManager()->pathTextures + imageName;

			this->mTexture = GlobalManager::getResourceManager()->loadTexture(imagePath.c_str());

			if (!this->mTexture) // Sanity checker.
				std::cout << "Warning: GLRect texture failed to deserialize!" << std::endl;
		}
		else
		{
			std::cout << "Warning: GLRect had a 'Texture' but its value was not a string!" << std::endl;
		}
	}

	// Try to deserialize color.
	if (rectObject.HasMember("Color"))
	{
		if (rectObject["Color"].IsArray())
		{
			// Get the array from the JSON...
			rapidjson::GenericArray<true,rapidjson::Value> colorArray = rectObject["Color"].GetArray();

			// Check array formatting.
			if (colorArray.Capacity() == 4 && colorArray[0].IsNumber())
			{
				// Formatting is ok. Set color.
				this->setColor(glm::vec4(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat(), colorArray[3].GetFloat()));
			}
			else
			{
				std::cout << "Warning: GLRect had a 'Color' but its array was incorrectly formatted!" << std::endl;
			}
		}
		else
		{
			std::cout << "Warning: GLRect had a 'Color' but its value was not an array of numbers!" << std::endl;
		}
	}
}