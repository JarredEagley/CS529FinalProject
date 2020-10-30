/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			main.cpp
	Purpose:			The main file for Milestone 2.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/5/2020

- End Header --------------------------------------------------------*/

#include <SDL.h>
#include "stdio.h"
#include <iostream>
#include <windows.h>
#include <fstream>

#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Managers/GlobalManager.h"

#include "Components/ComponentTypes.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc, char* args[])
{
	SDL_Window *pWindow;
	int error = 0;
	bool appIsRunning = true;
	SDL_Surface* pWindowSurface = NULL;

	// Framerate cap.
	GlobalManager::getFrameRateController()->setMaxFramerate(30);

	// Initialize a console.
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle(L"SDL 2.0 Test");
	}

	// Initialize SDL
	if((error = SDL_Init( SDL_INIT_VIDEO )) < 0 )
	{
		printf("Couldn't initialize SDL, error %i\n", error);
		return 1;
	}

	pWindow = SDL_CreateWindow("SDL2 window",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		1080,										// width, in pixels
		800,										// height, in pixels
		SDL_WINDOW_SHOWN);

	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// Get the window surface.
	pWindowSurface = SDL_GetWindowSurface(pWindow);

	// Update the window surface.
	SDL_UpdateWindowSurface(pWindow);

	// Create the game object factory.
	GameObjectFactory* pGOF = new GameObjectFactory();

	// Create game objects.
	//pGOF->loadObject(".\\Resources\\Player.json");
	pGOF->loadLevel(".\\Resources\\ExampleLevel.json");	 // Example for level deserialization.

	int posX = 0, posY = 0;
	float speed = 2;

	// Game loop
	while(true == appIsRunning)
	{
		GlobalManager::getFrameRateController()->frameStart();

		// Don't want this queue to fill up. Never put anything before it.
		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				appIsRunning = false;
			}
		} // Done handling events.

		// Input management. 
		GlobalManager::getInputManager()->Update();

		// Clear the window surface.
		SDL_FillRect(pWindowSurface, NULL, 0xFF444444);

		// Update
		for (auto pGameObject : GlobalManager::getGameObjectManager()->mGameObjects)
			pGameObject->Update();

		// Display the game objects.
		for (auto pGameObject : GlobalManager::getGameObjectManager()->mGameObjects)
		{
			// Not data-driving this yet as it'll all change anyways when I swap over to GLEW openGL graphics.
			Transform* pT = static_cast<Transform*>(pGameObject->GetComponent(ComponentTypes::TYPE_TRANSFORM)); // Next optimization for this class: GEt rid of this mess!
			Sprite* pS = static_cast<Sprite*>(pGameObject->GetComponent(ComponentTypes::TYPE_SPRITE));			// We want to data-drive this; move it outside the code!

			SDL_Rect destinationRect;
			destinationRect.x = static_cast<int>(pT->getX());
			destinationRect.y = static_cast<int>(pT->getY());
			SDL_BlitSurface(pS->mpSurface, NULL, pWindowSurface, &destinationRect);
		}

		SDL_UpdateWindowSurface(pWindow);
		
		// Quit
		InputManager *pIM = GlobalManager::getInputManager();
		if (pIM->IsKeyPressed(SDL_SCANCODE_Q) || pIM->IsKeyPressed(SDL_SCANCODE_ESCAPE))
			appIsRunning = false;
	
		// Lock the framerate.
		GlobalManager::getFrameRateController()->frameEnd();
	}

	delete pGOF;

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();
	
	return 0;
}