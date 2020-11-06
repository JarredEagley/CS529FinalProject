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
#include "GameObjectFactory.h" // TO-DO: Singleton?

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

	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to initialize SDL." << std::endl;
		return false;
	}

	
	// Create our window.
	GlobalManager::getGraphicsManager()->windowWidth = 1000;
	GlobalManager::getGraphicsManager()->windowHeight= 800;

	pWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		GlobalManager::getGraphicsManager()->windowWidth, GlobalManager::getGraphicsManager()->windowHeight, SDL_WINDOW_OPENGL);

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

	// Init the singletons.
	GlobalManager::initSingletons();
	// Set framerate.
	GlobalManager::getFrameRateController()->setMaxFramerate(30);

	// VERTEX TEST STUFF ---------
	// Create vertex array object.
	/*
	Shader shaderTest(".\\Shaders\\core.vert", ".\\Shaders\\core.frag");


	float verts[] = {
		// positions          // colors             // texture coords
		 0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0,1,3, // First triangle.
		1,2,3  // Second triangle.
	};
	
	// Vertex array object, vertex buffer object, element buffer object.
	GLuint vao, vbo, ebo;
	// Generate
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	// Bind
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int const number_of_values = 9;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, number_of_values * sizeof(GLfloat), (GLvoid*)0); // position attribute.
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, number_of_values * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // color attribute.
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, number_of_values * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat))); // UV attribute.
	glEnableVertexAttribArray(2);



	// Texture stuff...

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // try linear as well...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	// Load image
	int texwidth, texheight, nrChannels;
	stbi_uc *data = stbi_load("Resources\\Ship1.png", &texwidth, &texheight, &nrChannels, STBI_rgb_alpha);
	
	if (data)
	{
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D); // Mipmap might not be needed for my game...
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE!!!" << std::endl;
	}
	stbi_image_free(data); // We dont actually need the image anymore because its stored in openGL!

	glUniform1i(glGetUniformLocation(shaderTest.ProgramID, "ourTexture"), 0);
	//shaderTest.setInt("ourTexture", 0);

	shaderTest.Use();	
	*/

	GameObjectFactory* pGOF = new GameObjectFactory();
	pGOF->loadLevel(".\\Resources\\Levels\\ExampleLevel.json");

	// Alpha blend mode.

	// END OF VERTEX TEST STUFF ---------
	
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

		// Do updates.
		for (auto pGO : GlobalManager::getGameObjectManager()->mGameObjects)
			pGO->Update();
		
		// Clear the buffer.
		// The color I chose is just for fun.
		glClearColor(0.1, 0.11, 0.14, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// VERTEX TEST
		GlobalManager::getGraphicsManager()->drawAllGameObjects();


		SDL_GL_SwapWindow(pWindow);

		GlobalManager::getFrameRateController()->frameEnd();
	}
}

void Cleanup()
{
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