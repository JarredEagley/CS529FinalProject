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

enum class RenderPassType
{
	NONE,
	HUD,
	FINAL,

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

	// Performs all draw passes.
	void Draw();
	void DrawHUD();
	void DrawFinal();
	void drawAllGameObjects();  // Depricated.
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

public:
	static std::unordered_map<std::string, ShaderProgram*> mShaderPrograms; // shaderName, ShaderProgram*
	int mWindowWidth, mWindowHeight;

	// Different objects for different render passes.
	// HUD objects will be in the hud pass, which is rendered onto an FBO.
	static std::unordered_map<RenderPassType, std::list<GameObject*>> mRenderPasses;

private:
	GraphicsManager();

private:
	static GraphicsManager *instance;
	
	GameObject* pCurrentCameraGO = nullptr;

	static float mZoomLevel;
	static float mMinZoomLevel;
	static float mMaxZoomLevel;
};

