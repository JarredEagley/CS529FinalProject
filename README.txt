/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			README.txt
	Purpose:			Describes the proper usage of my final project
	Language:			ASCII
	Platform:			Notepad++, Windows 10
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/10/2020

- End Header --------------------------------------------------------*/

To compile and run, open the solution in Visual studio 2019 and run from main.cpp. an
example level will be deserialized from json format, stored in the Resources folder.

If this software is not compiled on Windows 10 using Visual Studio 2019, I cannot
predict the results, though I see no reason why other recent versions of Visual
Studio should not work.

If the included external libraries (one directory up) and my basic provided textures 
are not included with the project, I cannot predict the results. Most likely linker 
errors and a failure to load the image.
Both should be present to compile the project.

***************************************




 ----- The Resources Folder -----
 Resources contains many useful folders and files used for data driving the game.
 
 Config.json:
 Used in initial startup state for the game. Options such as max framerate and Windows
 size can be set in here. I have listed all the managers in there for future proofing,
 but not all of them will have paramaters to tweak.

Textures
Textures for the game. I'm using png files.

Levels
Level json files for the game. A level file can have a name, and a list of GameObjects.
GameObjects must have a name. GameObjects may have an Archetype, and a list of components.
Components which exist in the archetype may be overidden.

Archetypes
A special json file which designates a generic type of GameObject. An archetype cannot
be instantiated and therefore has no Name. It's basically just a bunch of components and
perhaps a render pass option to keep the level file clean.

Projectiles
A type of GameObject which can be dynamically spawned in mid-game with its own cap. These
must define a full GameObject and therefore have a Name parameter.

Particles
Similar to projectiles, but lower priority and purely for visual effect.


 ----- Render passes -----
In this version of the game, there are two render passes. There is the final pass and
the HUD pass. The final pass includes the camera view and perspective projections. The
HUD pass displays objects in normalized device coordinates with no distortion.
HUD objects are rendered after the final pass, allowing for a 2D display on top of the
game world.
