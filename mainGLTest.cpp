// This is a file for testing openGL.

// Based on a tutorial found at:
// https://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
// There's several ways to do this but I thought this tutorial's formatting was the cleanest.

#include <string>
#include <iostream>

//#define GL3_PROTOTYPES 1 // TO-DO What does this do?
#include <gl/glew.h>

#include <SDL.h>

#include "Shader.h"
#include "Managers/GlobalManager.h"

// Tutorial had this global. I thought it was a pretty good idea!
std::string programName = "CS529 Final Project - Jarred Eagley";

// TO-DO: I don't like what the tutorial did putting the window up here.
SDL_Window* pWindow;

// The OpenGL Context handle.
SDL_GLContext context;

GlobalManager globalManager;

// Shader sources
// TEMPORARY, better behavior in my old glfw implementation will be copied over later.
/*
const GLchar* vertexSource =
"attribute vec4 position;    \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = vec4(position.xyz, 1.0);  \n"
"}                            \n";
const GLchar* fragmentSource =
"precision mediump float;\n"
"void main()                                  \n"
"{                                            \n"
"  gl_FragColor = vec4 (1.0, 1.0, 1.0, 1.0 );\n"
"}                                            \n";
*/

// Some prototypes:
bool setOpenGLAttributes();
//void printSDL_GL_Attributes();
void checkSDLError(int line);
void gameLoop();
void Cleanup();

// The next two functions make main a little bit nicer. I followed a tutorial to get GLEW working with SDL 2.0.

bool Init()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to initialize SDL." << std::endl;
		return false;
	}


	// Create our window.
	pWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); // TO-DO: Do I need to set the color attributes and stuff?

	// Turn on double buffering.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

int main(int argc, char*argv[])
{
	// Initialize.
	if (!Init())
		return EXIT_FAILURE;

	GlobalManager::getFrameRateController()->setMaxFramerate(30);

	// VERTEX TEST STUFF ---------
	// Create vertex array object.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create Vertex buffer object, copy over vertex data.
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat verts[] = { 
		-0.4f, 0.5f, 0.0f,	1.0f, 0.5f, 0.1f, 1.0f, 
		0.5f, -0.5f, 0.0f,	1.0f, 0.5f, 0.1f, 0.3f,
		-0.5f, -0.5f, 0.0f,	1.0f, 0.5f, 0.1f, 0.0f,
		-0.4f, 0.5f, 0.0f,	0.1f, 0.5f, 1.0f, 1.0f, //
		0.4, 0.5, 0.0f,		0.1f, 0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.1f, 0.5f, 1.0f, 1.0f
	 };

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	Shader shaderTest("Shaders\\core.vert" , "Shaders\\core.frag");

	/*
	// Create and compile vertex shader.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL); // Shaders will eventually be separated out.
	glCompileShader(vertexShader);

	// Fragment shader's turn.
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link vertex shader and fragment shader into a shader program.
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data.
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	*/

	shaderTest.Use();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0); // position attribute.
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // color attribute.
	glEnableVertexAttribArray(1);

	/*
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	*/

	// Alpha blend mode.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// END OF VERTEX TEST STUFF ---------
	
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
		// TO-DO: Will  need to do frameStart here after I've got openGL running.

		// Don't want this queue to fill up. Never put anything before it.
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				appIsRunning = false;
			}
		} // Done handling events.

		// Do updates.
		// ...


		// Clear the buffer.
		// The color I chose is just for fun.
		glClearColor(0.1, 0.3, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// VERTEX TEST
		glDrawArrays(GL_TRIANGLES, 0, 6);

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

	// TO-DO: Will need to delete singletons here.
	GlobalManager::destroySingleton();
	globalManager.destroySingleton(); // TO-DO: Make name easier to use, switch over to this format so i can destroy singleton.
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