#include "GLRect.h"
#include "../Managers/GlobalManager.h"
#include "ComponentTypes.h"
#include <iostream>
//#include <string>

GLRect::GLRect() : scaleX(100.0f), scaleY(100.0f), Component(ComponentTypes::TYPE_GLRECT)
{
	mTexture = nullptr;
}


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

void GLRect::Serialize(rapidjson::Value::ConstMemberIterator inputMemberIt)
{
	// Sanity check for serializing GLRect.
	if (!inputMemberIt->value.IsString())
	{
		std::cerr << "Warning: Sprite failed to deserialize. Value was not a string." << std::endl;
		return;
	}

	const std::string imageName = inputMemberIt->value.GetString();
	const std::string imagePath = GlobalManager::getResourceManager()->pathTextures + imageName;

	mTexture = GlobalManager::getResourceManager()->loadTexture(imagePath.c_str());
}