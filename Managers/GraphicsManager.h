/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			GraphicsManager.h
	Purpose:			Responsible for tracking graphics-related activity.
						This includes, but is not limited to camera zoom level,
						what object is the camera, window size, and 
						drawing different objects in different render
						passes.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/4/2020

- End Header --------------------------------------------------------*/

#pragma once

#include "../GameObject.h"
#include "../ShaderProgram.h"
#include <unordered_map>
#include "../FrameBufferObject.h"
#include "GL/glew.h"

enum class RenderPassType
{
	NONE,
	BACKGROUND,
	MIDGROUND,
	FOREGROUND,
	HUD,

	NUM
};

// Handles shaders, drawing.
class GraphicsManager
{
public:
	static GraphicsManager* getInstance()
	{
		if (!instance)
			instance = new GraphicsManager;
		return instance;
	};
	void destroySingleton();

	void removeFromAnyRenderPasses(GameObject* pGO);
	void addToRenderPass(GameObject* pGO, RenderPassType newRenderPassType);

	RenderPassType stringToRenderPassType(std::string renderTypeName)
	{
		if (renderTypeName == "BACKGROUND") return RenderPassType::BACKGROUND;
		if (renderTypeName == "MIDGROUND") return RenderPassType::MIDGROUND;
		if (renderTypeName == "FOREGROUND") return RenderPassType::FOREGROUND;
		if (renderTypeName == "HUD") return RenderPassType::HUD;
		return RenderPassType::NONE; // failed
	}

	// Performs all draw passes.
	void Draw();
	void DrawHUD();
	void drawPass(RenderPassType passType);
	//void drawAllGameObjects();  // Depricated.
	void drawGameObject(GameObject *pGO);
	void drawGameObject_HUD(GameObject* pGO);

	ShaderProgram* loadShader(std::string shaderName); // Loads a shader program. Creates the program if it cannot be found.

	void setCurrentCameraGO(GameObject* pCam); // Cameras are game objects with a camera component.
	GameObject* getCurrentCameraGO() { return pCurrentCameraGO; };

	// Was originally just in camera, but I'll be needing this in more components.
	float getZoomLevel() { return mZoomLevel; };
	void setMaxZoomLevel(float zoomLevel);
	float getMaxZoomLevel();
	void setMinZoomLevel(float zoomLevel);
	float getMinZoomLevel();
	void setZoomLevel(float zoom);
	void incrementZoomLevel(float delta);

	unsigned int getVAORect();

public:
	static std::unordered_map<std::string, ShaderProgram*> mShaderPrograms; // shaderName, ShaderProgram*
	int mWindowWidth, mWindowHeight;

	// Different objects for different render passes.
	// HUD objects will be in the hud pass, which is rendered onto an FBO.
	static std::unordered_map<RenderPassType, std::list<GameObject*>> mRenderPasses;

private:
	GraphicsManager();

	void setUniformDefaults(ShaderProgram* pProgram); // Private function which sets default values for uniforms used in things like physics bodies.

	// GLRect VAO
	void buildVAORect();

private:
	static GraphicsManager *instance;
	
	GameObject* pCurrentCameraGO = nullptr;

	static float mZoomLevel;
	static float mMinZoomLevel;
	static float mMaxZoomLevel;


	// GLRect stuff-- We essentially have one quad that we stamp across the scene to save memory.
	GLuint vboIDRect[3];
	GLuint vaoIDRect; // The OpenGL identifier for the Vertex Array Object for this gameObject.
	// A 1x1 flat white square with default uv's.
	glm::vec4 vertPosRect[4] = {
		// Supporting 3d transfomrations. 
		glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),		// Left Top
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),	// Left Bot
		glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),		// Right bot
		glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),		// Right top
	};
	glm::vec2 vertUVRect[4] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
	};
	unsigned int indicesRect[6] = {
		0, 1, 3, // First triangle.
		1, 2, 3	 // Second triangle.
	};
};

