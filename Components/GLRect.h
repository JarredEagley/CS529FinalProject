#pragma once

#include "Component.h"

#include "../Managers/ResourceManager.h" // I really dont like having to do this to get my encapsulation. Need to find a better way. 

#include "glm/glm.hpp"
#include "stb_image.h"

class GLRect : public Component
{
public:
	GLRect();
	~GLRect();

	void Update();
	void setColor(glm::vec4); // Sets the color of the whole square. Will implement gradiant coloring if necessary.

	void buildVAO(); // Build this component's VAO. Automatically stores vaoId.
	unsigned int getVAO(); // Returns the iD of this component's vertex array object.
	int getTexId() { return texID; }; // Returns the ID of the texture stored on the graphics card which we want this component to draw.

	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);

public:
	// No public variables.
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

	unsigned int mIndices[6] = {
		0, 1, 3, // First triangle.
		1, 2, 3	 // Second triangle.
	};

	unsigned int vaoID; // The OpenGL identifier for the Vertex Array Object for this gameObject.
	int texID; // ID of the texture we want to use; stored on the graphics card.
};
