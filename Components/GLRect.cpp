#include "GLRect.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"
#include <iostream>

GLRect::GLRect() : Component(ComponentTypes::TYPE_GLRECT)
{
	std::cout << "DEBUG - Rect constructor fired." << std::endl;
	// Texture is null until loaded.
	//mTexture->texture = nullptr;
	vaoID = NULL;
	buildVAO();
}

GLRect::~GLRect()
{
}

void GLRect::Update()
{
}

void GLRect::setColor(glm::vec4 rgba)
{
	for (auto col : mVertCol)
		col = rgba;
}


// Builds the openGL vertex array object. 
void GLRect::buildVAO()
{
	// VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// VBO
	GLuint vboID[3];
	glGenBuffers(3, &vboID[0]);

	// Position, color, uv
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertPos) , &mVertPos[0][0] , GL_STATIC_DRAW ); 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1] );
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertCol) , &mVertCol[0][0] , GL_STATIC_DRAW );
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0 ); // TO-DO: Right stride?
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertUV) , &mVertUV[0][0] , GL_STATIC_DRAW );
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // TO-DO the 2, 4 might be wrong... 
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Pass in the texture...
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexture.width, mTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexture.texture);
	//glUniform1i(glGetUniformLocation(shaderTest.ProgramID, "ourTexture"), 0);
	//shaderTest.setInt("ourTexture", 0);

	// EBO
	GLuint eboID;
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * 2, mIndices, GL_STATIC_DRAW ); 

	glBindVertexArray(0); // Set back to default.
}

unsigned int GLRect::getVAO()
{
	if (vaoID == NULL)
		buildVAO();

	return vaoID;
}

void GLRect::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	std::cout << "DEBUG - Deserializing GLRect..." << std::endl;
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

			if (!this->mTexture->texture) // Sanity checker. // TO-DO: Will be wrong!
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