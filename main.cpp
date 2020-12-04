// This is a file for testing openGL.

// Based on a tutorial found at:
// https://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
// There's several ways to do this but I thought this tutorial's formatting was the cleanest.


#include <string>
#include <iostream>
#include <Windows.h>

//#define GL3_PROTOTYPES 1 // TO-DO What does this do?
#include <gl/glew.h>
#include <SDL.h>

#include "Managers/GlobalManager.h"
#include "Components/ComponentTypes.h"

#include "GameObject.h"

#define STB_IMAGE_IMPLEMENTATION // Couldn't find any good sources on why this is needed
#include "stb_image.h"
//#include "glm.hpp"


// Tutorial had this global. I thought it was a pretty good idea!
std::string programName = "CS529 Final Project - Jarred Eagley";

// Pointer to the SDL window.
SDL_Window* pWindow;

// The OpenGL Context handle.
SDL_GLContext context;

// Some prototypes:
bool setOpenGLAttributes();
//void printSDL_GL_Attributes();
void checkSDLError(int line);
void gameLoop();
void Cleanup();

// The next two functions make main a little bit nicer. I followed a tutorial to get GLEW working with SDL 2.0.

bool Init()
{
	// Initialize a console.
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle(L"Debug Console");
	}

	// Init my stuff.
	GlobalManager::initSingletons();
	GlobalManager::getGameStateManager()->readGameConfig();

	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to initialize SDL." << std::endl;
		return false;
	}
	
	// Create our window.
	pWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		GlobalManager::getGraphicsManager()->mWindowWidth, GlobalManager::getGraphicsManager()->mWindowHeight, SDL_WINDOW_OPENGL);

	// Nullcheck the window.
	if (!pWindow)
	{
		std::cerr << "Failed to create SDL window." << std::endl;
		checkSDLError(__LINE__);
		return false;
	}


	// Create the OpenGL Context, attach it to the window.
	context = SDL_GL_CreateContext(pWindow);

	// Set the openGL attributes, handled in the below function.
	// I've found conflicting information on whether this should be here or before the window creation... 
	// Seems to work here though.
	setOpenGLAttributes();

	// A magical SDL function which syncs buffer swap with monitor vertical refresh. Not quite vsync from my understanding, but similar.
	//https://wiki.libsdl.org/SDL_GL_SetSwapInterval
	// I'm keeping it as synced for now... Immediate may end up being a better option.
	SDL_GL_SetSwapInterval(1);

	// Initialize GLEW.
	glewExperimental = GL_TRUE;
	glewInit();

	return true;
}

bool setOpenGLAttributes()
{
	// Set OpenGL Version-- I'll do 3.2 for now. Theoretically 3.3 should also work just fine.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // TO-DO: Do I need to set the color attributes and stuff?

	// Turn on double buffering.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

int main(int argc, char*argv[])
{
	// Initialize.
	if (!Init())
		return EXIT_FAILURE;

	// Load level.
	GlobalManager::getResourceManager()->loadLevel("ExampleLevel.json");

	// Alpha blend.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // TO-DO: Where does this actually belong? init?
	glEnable(GL_BLEND);

	// Run the game loop.
	gameLoop();

	// Cleanup.
	Cleanup();

	return EXIT_SUCCESS;
}

void gameLoop()
{
	bool appIsRunning = true;
	while (true == appIsRunning)
	{
		GlobalManager::getFrameRateController()->frameStart();

		// ----- Input ----- //

		// Handle inputs...
		GlobalManager::getInputManager()->Update(); 

		// Don't want this queue to fill up. Never put anything before it.
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				appIsRunning = false;
			}
			if (e.type == SDL_MOUSEWHEEL)
			{
				GlobalManager::getInputManager()->recieveEvent(e);
			}
		} // Done handling events.

		GlobalManager::getEventManager()->Update();

		// ----- Updates ----- //

		// Do physics updates.
		GlobalManager::getPhysicsManager()->Update();

		// Do GameObject updates.
		GlobalManager::getGameObjectManager()->updateGameObjects();

		// TO-DO: Move this somewhere more sensible and make it more flexible.
		if (GlobalManager::getPhysicsManager()->isPhysicsPaused)
			GlobalManager::getPhysicsManager()->isPhysicsPaused = false;

		// ----- Drawing ----- //

		// Clear the buffer.
		// The color I chose is just for fun.
		glClearColor(0.1, 0.11, 0.14, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the game object VAO's.
		GlobalManager::getGraphicsManager()->Draw();

		SDL_GL_SwapWindow(pWindow); // Swaps the buffered frame to view.

		// Mutate mGameObjects for this game loop.
		GlobalManager::getGameObjectManager()->deleteRemovedGameObjects();
		GlobalManager::getGameObjectManager()->addCreatedGameObjects();

		GlobalManager::getFrameRateController()->frameEnd();
	}
}

void Cleanup()
{
	GlobalManager::getGameObjectManager()->deleteAllGameObjects();

	// Delete OpenGL context.
	SDL_GL_DeleteContext(context);

	// Destroy the window.
	SDL_DestroyWindow(pWindow);

	// Shut down SDL.
	SDL_Quit();  // TO-DO: I think I need to do this after deleting singletons.

	// Destroy all singletons.
	GlobalManager::destroySingleton();
}

/*
	The tutorial did not cover this function, but it basically just outputs any potential errors.
*/
void checkSDLError(int line = -1)
{
	std::string error = SDL_GetError();
	
	if (error != "")
	{
		std::cerr << "SDL Error: " << error << std::endl;

		if (line != -1)
			std::cerr << "\nLine: " << line << std::endl;

		SDL_ClearError();
	}
}

/* I'll implement this function if I end up needing it.
void printSDL_GL_Attributes()
{

}
*/