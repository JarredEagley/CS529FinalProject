#include "GLRect.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"
#include <iostream>

GLRect::GLRect() : Component(ComponentTypes::TYPE_GLRECT), 
mColor(glm::vec4(1.0f)), mUvScale(1.0f), mUvOffset(glm::vec2(0.0f))
{
	// Texture is null until loaded.
	//mTexture->texture = nullptr;
	vaoID = NULL;
	buildVAO();
}

GLRect::~GLRect()
{
	// Dispose of vertex data.
	glDeleteBuffers(3, &vboID[0]);
	glDeleteVertexArrays(1, &vaoID);
}

void GLRect::Update()
{
}

void GLRect::setColor(glm::vec4 rgba)
{
	// Store the color so we can ask GLRect for it.
	mColor = rgba;
	/*
	// Set the vert colors.
	for (int i = 0; i < 4; ++i)
	{
		mVertCol[i] = rgba;
	}
	// Build the new VAO.
	rebuildVAO();
	*/
}


void GLRect::setUvScale(float scale)
{
	// Keep track.
	this->mUvScale = scale;
	
	/*
	// Actually change.
	mVertUV[0] = glm::vec2(0.0f, scale);
	mVertUV[1] = glm::vec2(0.0f, 0.0f);
	mVertUV[2] = glm::vec2(scale, 0.0f);
	mVertUV[3] = glm::vec2(scale, scale);

	// Build the new VAO.
	rebuildVAO();
	*/
}

float GLRect::getUvScale()
{
	return this->mUvScale;
}

void GLRect::setUvOffset(glm::vec2 offset)
{
	this->mUvOffset = offset;

	/*
	// Actually change.
	mVertUV[0] += offset;
	mVertUV[1] += offset;
	mVertUV[2] += offset;
	mVertUV[3] += offset;

	// Build the new VAO.
	rebuildVAO();
	*/
}

glm::vec2 GLRect::getUvOffset()
{
	return this->mUvOffset;
}


// PRIVATE.
// Builds the openGL vertex array object. 
void GLRect::buildVAO()
{
	// VAO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// VBO
	glGenBuffers(2, &vboID[0]);

	// Position, color, uv
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertPos) , &mVertPos[0][0] , GL_STATIC_DRAW ); 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1] );
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertCol) , &mVertCol[0][0] , GL_STATIC_DRAW );
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0 ); 
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertUV) , &mVertUV[0][0] , GL_STATIC_DRAW );
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
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


// Binds the uniform data associated with this paticular GLRect.
void GLRect::setUniformData(ShaderProgram* pProgram)
{
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "uvScale");
	glUniform1f(loc, getUvScale());
	loc = glGetUniformLocation(pProgram->ProgramID, "uvOffset");
	glUniform2fv(loc, 1, &mUvOffset.x );
	loc = glGetUniformLocation(pProgram->ProgramID, "color");
	glUniform4fv(loc, 1, &mColor.x );
}


void GLRect::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	//std::cout << "DEBUG - Deserializing GLRect..." << std::endl;
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

			this->texID = GlobalManager::getResourceManager()->loadTexture(imagePath.c_str());

			if (this->texID == NULL) // Sanity checker. 
				std::cout << "Warning: GLRect texture '"<< imagePath << "' failed to deserialize!" << std::endl;
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
				glm::vec4 ourcolor = glm::vec4(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat(), colorArray[3].GetFloat());
				this->setColor(ourcolor);
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