#pragma once

#include "Component.h"

#include "glm/glm.hpp"
#include "stb_image.h"
#include "../Shader.h"

struct Vertex
{
	glm::vec4 position; // Going to support 3D position even though I'll never use it.
	glm::vec4 color;
	glm::vec2 uv;
};

class GLRect : public Component
{
public:
	GLRect();
	~GLRect();

	void Update();
	void setColor(glm::vec4); // Sets the color of the whole square. Will implement gradiant coloring if necessary.

	void Draw(Shader* shader);
	void buildVAO();

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	stbi_uc* mTexture;
private:
	// No private methods.
private:
	// A 1x1 flat white square with default uv's.
	Vertex vertices[4]; // Initialized in constructor.
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
	unsigned int mIndices[6] = { // TO-DO These might be wrong now...
		0,1,3, // First triangle.
		1,2,3  // Second triangle.
	};

	// The OpenGL identifier for the Vertex Array Object for this gameObject.
	// VAO stuff may eventually be abstracted into its own component.
	unsigned int vaoID;
};

