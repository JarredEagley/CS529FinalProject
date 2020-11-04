#pragma once


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

	void drawGameObject();

public:
	// No public variables.
private:
	GraphicsManager();
private:
	static GraphicsManager *instance;
};

