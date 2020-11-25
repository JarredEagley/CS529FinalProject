#include "GraphicsManager.h"

#include "stb_image.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/GLRect.h" // May rename to 'GLGraphic' or something in the future... Not sure.
#include "../Components/Transform.h"
#include "../Components/Camera.h"

#include "glm/gtc/type_ptr.hpp"

GraphicsManager* GraphicsManager::instance = nullptr;
std::unordered_map<const char*, ShaderProgram*> GraphicsManager::mShaderPrograms;

bool toggle = false; // TO-DO: This is for milestone 3. Remove when done.

void GraphicsManager::destroySingleton()
{
	// Destroy all shader programs.
	for (auto keyValuePair : mShaderPrograms)
		delete keyValuePair.second;

	// Clear the hash map.
	mShaderPrograms.clear();

	delete instance;
}

GraphicsManager::GraphicsManager()
{
	pCurrentCamera = nullptr;
	windowHeight = 0;
	windowWidth = 0;
}

// Draws every GameObject stored in the GameObjectManger.
void GraphicsManager::drawAllGameObjects()
{
	// TEMPORARY -- the debug draw toggle for milestone 3
	bool spacebar = GlobalManager::getInputManager()->IsKeyTriggered(SDL_SCANCODE_SPACE);
	if (spacebar)
	{
		std::cout << "Toggling milestone 3 draw mode..." << std::endl;
		toggle = !toggle;
	}

	GameObjectManager *pGOM = GlobalManager::getGameObjectManager();
	for (auto pGOPair : pGOM->mGameObjects)
	{
		drawGameObject(pGOPair.second);
	}
}

// Draws the given GameObject pointed to.
void GraphicsManager::drawGameObject(GameObject* pGO)
{
	// Get the shader we want to use.
	ShaderProgram* program = loadShader(pGO->mShaderName);

	// Components we'll need.
	GLRect* pRect = static_cast<GLRect*>(pGO->GetComponent(ComponentTypes::TYPE_GLRECT)); // From game object being drawn
	Transform* pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // From game object being drawn

	// Can't draw something with no graphics component.
	if (pRect == nullptr)
		return;

	//Transform* pCameraTransform = nullptr;
	Camera* pCamera = nullptr;
	if (pCurrentCamera == nullptr)
	{
		std::cout << "Error: No camera GameObject currently bound to the GraphicsManager." << std::endl;
	}
	else
	{
		//pCameraTransform = static_cast<Transform*>(pCurrentCamera->GetComponent(ComponentTypes::TYPE_TRANSFORM));
		pCamera = static_cast<Camera*>(pCurrentCamera->GetComponent(ComponentTypes::TYPE_CAMERA)); // From bound camera game object.
	}

	// Get the vaoID we want to draw.
	unsigned int vaoID = pRect->getVAO(); // NOT ALL OBJECTS HAVE VAOS.

	if (program == nullptr)
	{
		std::cout << "Warning: GameObject " << pGO->mName << " had an invalid shaderName. Aborting draw." << std::endl;
		return;
	}

	//glm::mat4 cameraTrasform = pCamera->getRotMatrix() * pCameraTransform->getTransformationMatrix(); // Separated out rotation for now.

	// --- Draw --- //

	// bind
	program->Use();
	glBindVertexArray(vaoID); // Bind the VAO
	glActiveTexture(GL_TEXTURE0); // Will be needed if I one day one multiple textures on one rect.
	glBindTexture(GL_TEXTURE_2D, pRect->getTexId()); // Bind the desired texture.

	unsigned int loc;

	// transform
	loc = glGetUniformLocation(program->ProgramID, "transform");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr( pTransform->getTransformationMatrix() ));
	// Cam transforms
	loc = glGetUniformLocation(program->ProgramID, "viewTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(pCamera->getTransMatrix() ));
	loc = glGetUniformLocation(program->ProgramID, "viewProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr( pCamera->getProjMatrix() ));

	loc = glGetUniformLocation(program->ProgramID, "debugDraw");
	glUniform1i(loc, toggle);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // <--- The actual draw call! <---


	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	program->unUse();
}

ShaderProgram* GraphicsManager::loadShader(const char* shaderName)
{
	// Test if the shader by the given name exists. If it does, return it.
	if ( mShaderPrograms.find(shaderName) != mShaderPrograms.end() ) // May have bad efficiency. Could be worth exploring alternate ways of doign this.
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
		return nullptr;
	}

	return program;
}
