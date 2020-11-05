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

public:
	std::unordered_map<const char*, ShaderProgram*> mShaderPrograms; // shaderName, ShaderProgram*
	
private:
	GraphicsManager();
private:
	static GraphicsManager *instance;
	GameObject* pCurrentCamera;
};

