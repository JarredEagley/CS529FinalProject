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
}

void GLRect::setUvScale(float scale)
{
	// Keep track.
	this->mUvScale = scale;
}

float GLRect::getUvScale()
{
	return this->mUvScale;
}

void GLRect::setUvOffset(glm::vec2 offset)
{
	this->mUvOffset = offset;
}

glm::vec2 GLRect::getUvOffset()
{
	return this->mUvOffset;
}

// Asks the ResourceManager for the provided texture and applies it to this rect.
void GLRect::setTexture(const char* imageName)
{
	const std::string imagePath = GlobalManager::getResourceManager()->pathTextures + imageName;
	GLuint newTexId = GlobalManager::getResourceManager()->loadTexture(imagePath.c_str());

	if (newTexId == NULL) // Sanity checker. 
		std::cout << "Warning: GLRect texture '" << imagePath << "' failed to deserialize!" << std::endl;

	mTexID = newTexId;
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

	// Position, uv
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertPos) , &mVertPos[0][0] , GL_STATIC_DRAW ); 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Color is nolonger a vertex attribute.

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
			this->setTexture(rectObject["Texture"].GetString());
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