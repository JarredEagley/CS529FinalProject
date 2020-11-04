#include "GraphicsManager.h"

#include "stb_image.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/GLRect.h" // May rename to 'GLGraphic' or something in the future... Not sure.

void GraphicsManager::destroySingleton()
{
	// Destroy all shader programs.
	for (auto keyValuePair : mShaderPrograms)
		delete keyValuePair.second;

	// Clear the hash map.
	mShaderPrograms.clear();
}

// Draws every GameObject stored in the GameObjectManger.
void GraphicsManager::drawAllGameObjects()
{
	GameObjectManager *pGOM = GlobalManager::getGameObjectManager();
	for (auto pGO : pGOM->mGameObjects)
	{
		drawGameObject(pGO);
	}
}

// Draws the given GameObject pointed to.
void GraphicsManager::drawGameObject(GameObject* pGO)
{
	// Get the shader we want to use.
	ShaderProgram* program = loadShader(pGO->shaderName);

	// Get the vaoID we want to draw.
	GLRect* pRect = static_cast<GLRect*>(pGO->GetComponent(ComponentTypes::stringToEnum("TYPE_GLRECT")));
	unsigned int vaoID = pRect->getVAO();

	if (program == nullptr)
	{
		std::cout << "Warning: GameObject " << pGO->getID() << " had an invalid shaderName. Aborting draw." << std::endl;
		return;
	}

	// Draw.
	program->Use();
	glBindVertexArray(vaoID); // Bind the VAO
	glActiveTexture(GL_TEXTURE0); // Will be needed if I one day one multiple textures on one rect.
	glBindTexture(GL_TEXTURE_2D, pRect->getTexId()); // Bind the desired texture.

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	program->unUse();

}

ShaderProgram* GraphicsManager::loadShader(const char* shaderName)
{
	// Test if the shader by the given name exists. If it does, return it.
	if ( mShaderPrograms.find(shaderName) == mShaderPrograms.end() ) // May have bad efficiency. Could be worth exploring alternate ways of doign this.
		return mShaderPrograms[shaderName];

	// Shader by that name doesn't exist. Try to create it.
	std::string vertpath = ".\\Shaders\\"; // Move into RM?
	vertpath = vertpath + shaderName;
	vertpath = vertpath + ".vert";
	std::string fragpath = ".\\Shaders\\"; // Move into RM?
	fragpath = fragpath + shaderName;
	fragpath = fragpath + ".frag";

	ShaderProgram* program = mShaderPrograms[shaderName] = new ShaderProgram(vertpath.c_str(), fragpath.c_str());
	if (!program)
	{
		std::cerr << "Error: Shader program '" << shaderName << "' failed to build." << std::endl;
		delete mShaderPrograms[shaderName];
		mShaderPrograms.erase(shaderName);
		return;
	}

	return program;
}

