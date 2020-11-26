#pragma once

#include "../GameObject.h"
#include "../ShaderProgram.h"
#include <unordered_map>

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

	void drawAllGameObjects();
	void drawGameObject(GameObject *pGO);

	ShaderProgram* loadShader(const char* shaderName); // Loads a shader program. Creates the program if it cannot be found.

	void setCurrentCamera(GameObject* pCam) { pCurrentCamera = pCam; }; // Cameras are game objects with a camera component.

	// Was originally just in camera, but I'll be needing this in more components.
	float getZoomLevel() { return mZoomLevel; };
	void setMaxZoomLevel(float zoomLevel);
	float getMaxZoomLevel() { return mMaxZoomLevel; };
	void setMinZoomLevel(float zoomLevel);
	float getMinZoomLevel() { return mMinZoomLevel; };
	void setZoomLevel(float zoom);
	void incrementZoomLevel(float delta);

public:
	static std::unordered_map<const char*, ShaderProgram*> mShaderPrograms; // shaderName, ShaderProgram*
	int windowWidth, windowHeight;

private:
	GraphicsManager();
	float mZoomLevel;
	float mMinZoomLevel;
	float mMaxZoomLevel;
private:
	static GraphicsManager *instance;
	GameObject* pCurrentCamera = nullptr;;
};

