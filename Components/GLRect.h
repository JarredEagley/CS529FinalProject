#pragma once

#include "Component.h"

#include "../Managers/ResourceManager.h" // I really dont like having to do this to get my encapsulation. Need to find a better way. 

#include "glm/glm.hpp"
#include "stb_image.h"
#include "../Shader.h"

class GLRect : public Component
{
public:
	GLRect();
	~GLRect();

	void Update();
	void setColor(glm::vec4); // Sets the color of the whole square. Will implement gradiant coloring if necessary.

	void Draw(Shader* shader) {};
	void buildVAO();

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	stbiTexture* mTexture; // TO-DO: Textures will be on the GRAPHICS CARD...

	Shader shaderTest = Shader(".\\Shaders\\core.vert", ".\\Shaders\\core.frag"); // TO-DO: This is NOT the place for this!!!

private:
	// No private methods.
private:
	// A 1x1 flat white square with default uv's.
	glm::vec4 mVertPos[4] = {
		// Supporting 3d transfomrations. Theoretically I should never need them, but I don't know for sure.
		glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),		// Left Top
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),	// Left Bot
		glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),		// Right bot
		glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),		// Right top
	};
	glm::vec4 mVertCol[4] = { 
		// RGBA
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	glm::vec2 mVertUV[4] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
	};

	// Monolithic format (Depricated)
	/*
	float mVertices[36] = {
	//  ( X      Y     Z  )  ( R     G     B     A  )  ( U     V  )
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
	};
	*/
	unsigned int mIndices[6] = {
		0, 1, 3, // First triangle.
		1, 2, 3	 // Second triangle.
	};

	// The OpenGL identifier for the Vertex Array Object for this gameObject.
	// VAO stuff may eventually be abstracted into its own component.
	unsigned int vaoID;
};
