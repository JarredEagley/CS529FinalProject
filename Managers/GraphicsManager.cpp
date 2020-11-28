/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GraphicsManager.cpp
	Purpose:			Implementations for GraphicsManager.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/4/2020

- End Header --------------------------------------------------------*/

#include "GraphicsManager.h"

#include "stb_image.h"
#include "GlobalManager.h"
#include "../Components/ComponentTypes.h"
#include "../Components/GLRect.h" // May rename to 'GLGraphic' or something in the future... Not sure.
#include "../Components/Transform.h"
#include "../Components/Camera.h"

#include "glm/gtc/type_ptr.hpp"

// Static initializations.
GraphicsManager* GraphicsManager::instance = nullptr;
std::unordered_map<const char*, ShaderProgram*> GraphicsManager::mShaderPrograms;
float GraphicsManager::mMinZoomLevel = 0.1f;
float GraphicsManager::mMaxZoomLevel = 100000.0f;
float GraphicsManager::mZoomLevel = mZoomLevel;
std::unordered_map<GraphicsManager::RenderPassType, std::list<GameObject*>> GraphicsManager::mRenderPasses;

void GraphicsManager::destroySingleton()
{
	// Destroy all shader programs.
	for (auto keyValuePair : mShaderPrograms)
		delete keyValuePair.second;

	// Destroy render pass lists.
	for (auto rp : mRenderPasses)
		rp.second.clear();
	mRenderPasses.clear();

	// Clear the hash map.
	mShaderPrograms.clear();

	delete instance;
}

GraphicsManager::GraphicsManager() 
{
	pCurrentCameraGO = nullptr;
	mWindowHeight = 0;
	mWindowWidth = 0;
}

void GraphicsManager::addToRenderPass(GameObject* pGO, RenderPassType pass)
{
	// Check for invalid pass name.
	if (pass > RenderPassType::NUM)
	{
		std::cout << "Warning: GameObject" << pGO->mName << " was passed an invalid render pass type." << std::endl;
		return;
	}

	// No need to set the same renderpass.
	if (pGO->getRenderPass() == pass)
		return;

	// Remove from current pass.
	mRenderPasses[pGO->getRenderPass()].remove(pGO);

	// Add to new pass.
	mRenderPasses[pass].push_back(pGO);
}

// Draws every GameObject stored in the GameObjectManger.
void GraphicsManager::drawAllGameObjects()
{
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
	ShaderProgram* pProgram = loadShader(pGO->mShaderName);

	// Components we'll need.
	GLRect* pRect = static_cast<GLRect*>(pGO->GetComponent(ComponentTypes::TYPE_GLRECT)); // From game object being drawn
	Transform* pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // From game object being drawn


	// Can't draw something with no graphics component.
	if (pRect == nullptr)
		return;

	
	// Camera needs to exist.
	Camera* pCamera = nullptr;
	if (pCurrentCameraGO == nullptr)
		std::cout << "Error: No camera GameObject currently bound to the GraphicsManager." << std::endl;
	else
		pCamera = static_cast<Camera*>(pCurrentCameraGO->GetComponent(ComponentTypes::TYPE_CAMERA)); // From bound camera game object.


	// Get the vaoID we want to draw.
	GLuint vaoID = pRect->getVAO(); // Note: it's possible for an object to not have a VAO.


	if (pProgram == nullptr)
	{
		std::cout << "Warning: GameObject " << pGO->mName << " had an invalid shaderName. Aborting draw." << std::endl;
		return;
	}

	// --- Draw --- //

	// bind
	pProgram->Use(); // Use current program.
	glBindVertexArray(vaoID); // Bind the VAO
	glActiveTexture(GL_TEXTURE0); // Will be needed if I one day one multiple textures on one rect.
	glBindTexture(GL_TEXTURE_2D, pRect->getTexId()); // Bind the desired texture.
	pRect->setUniformData(pProgram);

	unsigned int loc;

	// transform
	loc = glGetUniformLocation(pProgram->ProgramID, "transform");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr( pTransform->getTransformationMatrix() ));
	// Cam transforms
	loc = glGetUniformLocation(pProgram->ProgramID, "viewTrans");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(pCamera->getTransMatrix() ));
	loc = glGetUniformLocation(pProgram->ProgramID, "viewProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr( pCamera->getProjMatrix() ));


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // <--- The actual draw call! <---


	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	pProgram->unUse();
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


void GraphicsManager::setCurrentCameraGO(GameObject* pCam)
{
	pCurrentCameraGO = pCam;
}


void GraphicsManager::setMaxZoomLevel(float zoomLevel)
{
	mMaxZoomLevel = zoomLevel;
	mZoomLevel = std::min(mMaxZoomLevel, mZoomLevel); // Clamp
}

void GraphicsManager::setMinZoomLevel(float zoomLevel)
{
	mMinZoomLevel = zoomLevel;
	mZoomLevel = std::max(mMinZoomLevel, mZoomLevel); // Clamp
}

void GraphicsManager::setZoomLevel(float zoom)
{
	mZoomLevel = zoom;
	mZoomLevel = std::max(mMinZoomLevel, mZoomLevel); // Clamp
	mZoomLevel = std::min(mMaxZoomLevel, mZoomLevel); // Clamp
}

void GraphicsManager::incrementZoomLevel(float delta)
{
	mZoomLevel += delta;
	mZoomLevel = std::max(mMinZoomLevel, mZoomLevel); // Clamp
	mZoomLevel = std::min(mMaxZoomLevel, mZoomLevel); // Clamp
}

