#pragma once

#include "Component.h"

//#include "stb_image.h"
typedef stbi_uc; // Forward declaration.

class GLRect : public Component
{
public:
	GLRect();
	~GLRect();

	void Update();
	void setColor(float r, float g, float b, float a); // Sets the color of the whole square. Will implement gradiant coloring if necessary.
	virtual void Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt);
public:
	stbi_uc* mTexture;
	float scaleX;
	float scaleY;
private:
	// No private methods.
private:
	// A 1x1 flat white square with default uv's.
	float mVertices[36] = {
	//  ( X      Y     Z  )  ( R     G     B     A  )  ( U     V  )
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int mIndices[6] = {
		0,1,3, // First triangle.
		1,2,3  // Second triangle.
	};
};

