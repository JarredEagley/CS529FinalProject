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
#include "../Components/ComponentTypes.h"
#include "../Components/GLRect.h" // May rename to 'GLGraphic' or something in the future... Not sure.
#include "../Components/Transform.h"
#include "../Components/Camera.h"

#include "glm/gtc/type_ptr.hpp"

// Static initializations.
GraphicsManager* GraphicsManager::instance = nullptr;
std::unordered_map<std::string, ShaderProgram*> GraphicsManager::mShaderPrograms;
float GraphicsManager::mMinZoomLevel = 0.1f;
float GraphicsManager::mMaxZoomLevel = 100000.0f;
float GraphicsManager::mZoomLevel = 100.0f;
std::unordered_map<RenderPassType, std::list<GameObject*>> GraphicsManager::mRenderPasses;

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

// --- End of singleton stuff --- //

GraphicsManager::GraphicsManager() : 
	pCurrentCameraGO(nullptr), 
	mWindowHeight(0), mWindowWidth(0)
{
}

void GraphicsManager::removeFromAnyRenderPasses(GameObject* pGO)
{
	mRenderPasses[ pGO->getRenderPassType() ].remove(pGO);
}

void GraphicsManager::addToRenderPass(GameObject* pGO, RenderPassType newRenderPassType)
{
	// Check for invalid pass name.
	if (newRenderPassType > RenderPassType::NUM)
	{
		std::cout << "Warning: GameObject" << pGO->mName << " was passed an invalid render pass type." << std::endl;
		return;
	}

	// No need to set the same renderpass.
	if (pGO->getRenderPassType() == newRenderPassType)
		return;

	// Remove from current pass.
	mRenderPasses[pGO->getRenderPassType()].remove(pGO);

	pGO->mRenderPassType = newRenderPassType; // Inform the game object.

	// Add to new pass.
	mRenderPasses[newRenderPassType].push_back(pGO);
}


void GraphicsManager::setUniformDefaults(ShaderProgram* pProgram)
{
	unsigned int loc;
	loc = glGetUniformLocation(pProgram->ProgramID, "col_type");
	glUniform1i(loc, 0); // 0 for collision type none.
}


// Draws every GameObject stored in the GameObjectManger.
/*
void GraphicsManager::drawAllGameObjects()
{
	GameObjectManager *pGOM = GlobalManager::getGameObjectManager();
	for (auto pGOPair : pGOM->mGameObjects)
	{
		drawGameObject(pGOPair.second);
	}
}
*/

// If I had to do many many draw passes refactoring this code into something more generic
// would be a good idea. This should be fine for my game, though.
void GraphicsManager::Draw()
{
	DrawFinal();
	DrawHUD();
}

void GraphicsManager::DrawHUD()
{
	// Loop through the GO's for this pass...
	for (auto pGO : mRenderPasses[RenderPassType::HUD])
	{
		if (pGO == nullptr)
			continue;
		drawGameObject_HUD(pGO);
	}
}

void GraphicsManager::DrawFinal()
{
	// Loop through the GO's for this pass...
	for (auto pGO : mRenderPasses[RenderPassType::FINAL])
	{
		if (pGO == nullptr)
			continue;
		drawGameObject(pGO);
	}
}

void GraphicsManager::drawGameObject_HUD(GameObject* pGO)
{
	// Get the shader we want to use.
	ShaderProgram* pProgram = loadShader(pGO->mShaderName);
	if (pProgram == nullptr)
	{
		std::cout << "Warning: GameObject " << pGO->mName << " had an invalid shaderName. Aborting draw." << std::endl;
		return;
	}


	// Components we'll need.
	Transform* pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // From game object being drawn
	if (pTransform == nullptr)
	{
		std::cout << "Warning: Aborting draw for " << pGO->mName << " because it did not have a transform component." << std::endl;
		return;
	}

	// --- Draw --- //

	// bind
	pProgram->Use(); // Use current program.
	setUniformDefaults(pProgram);

	pGO->setAllUniformData(pProgram);
	pGO->Draw(pProgram, 
		pTransform->getTransformationMatrix(), 
		glm::mat4(1.0f),
		glm::mat4(1.0f));

	pProgram->unUse();

}

// Draws the given GameObject pointed to.
void GraphicsManager::drawGameObject(GameObject* pGO)
{
	// Get the shader we want to use.
	ShaderProgram* pProgram = loadShader(pGO->mShaderName);
	if (pProgram == nullptr)
	{
		std::cout << "Warning: GameObject " << pGO->mName << " had an invalid shaderName. Aborting draw." << std::endl;
		return;
	}

	// Components we'll need.
	Transform* pTransform = static_cast<Transform*>(pGO->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // From game object being drawn

	// Camera needs to exist.
	Camera* pCamera = nullptr;
	if (pCurrentCameraGO == nullptr)
		std::cout << "Error: No camera GameObject currently bound to the GraphicsManager." << std::endl;
	else
		pCamera = static_cast<Camera*>(pCurrentCameraGO->GetComponent(ComponentTypes::TYPE_CAMERA)); // From bound camera game object.


	// --- Draw --- //

	// bind
	pProgram->Use(); // Use current program.
	setUniformDefaults(pProgram);

	pGO->setAllUniformData(pProgram);
	pGO->Draw(pProgram, 
		pTransform->getTransformationMatrix(),
		pCamera->getTransMatrix(), // View matrix
		pCamera->getProjMatrix());

	pProgram->unUse();
}


ShaderProgram* GraphicsManager::loadShader(std::string shaderName)
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

	//std::cout << "Shader " << shaderName << " failed to be found." << std::endl;

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

float GraphicsManager::getMaxZoomLevel()
{
	return mMaxZoomLevel;
}

void GraphicsManager::setMinZoomLevel(float zoomLevel)
{
	mMinZoomLevel = zoomLevel;
	mZoomLevel = std::max(mMinZoomLevel, mZoomLevel); // Clamp
}

float GraphicsManager::getMinZoomLevel()
{
	return mMinZoomLevel;
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

/*
#include "freetype/freetype.h"
void GraphicsManager::initializeCharacterMap()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Error::FreeType: Could not initialize FreeType Library." << std::endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		std::cout << "Error::Freetype: Failed to load font." << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "Error::FreeType: Failed to load Glyph. " << std::endl;
		return;
	}


}
*/