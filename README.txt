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

MILESTONE 4 NOTES:
Alongside my core gameplay events such as collision, I have set up a handful of demonstration game event triggers.
 - hold left click to spawn in a projectile (targeted event to subscribers)
 - press right click, after two seconds all projectile GameObjects will be deleted. (timed event)
 - press space and a global event will be broadcasted telling all GameObjects with physics bodies to spin. (global event)
There is also collision, which is an event which only two gameobjects may recieve.
 
 ***************************************
 
This is still heavily work in progress, bugs may be possible.