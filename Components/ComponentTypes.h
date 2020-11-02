/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			ComponentTypes.cpp
	Purpose:			A header file I created to simplify keeping track
						of the types associated with components.
						This includes the enums, as well as a simple little
						function which converts strings to their associated
						enum.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/22/2020

- End Header --------------------------------------------------------*/

#pragma once
#include <string.h>

class ComponentTypes
{
public:
	static unsigned int stringToEnum(const char* str)
	{
		// Wish I could do this with a switch case block it would be so much prettier.
		if (strcmp(str, "TYPE_TRANSFORM") == 0)			return TYPE_TRANSFORM;
		if (strcmp(str, "TYPE_SPRITE") == 0)			return TYPE_SPRITE;
		if (strcmp(str, "TYPE_CONTROLLERSLIDER") == 0)	return TYPE_CONTROLLERSLIDER;
		if (strcmp(str, "TYPE_UPDOWN") == 0)			return TYPE_UPDOWN;
		if (strcmp(str, "TYPE_GLRECT") == 0)			return TYPE_GLRECT;

		return TYPE_UNDEFINED;
	}
public:
	// Types of component.
	static enum COMPONENT_TYPE
	{
		TYPE_UNDEFINED,
		TYPE_TRANSFORM,
		TYPE_SPRITE,
		TYPE_CONTROLLERSLIDER,
		TYPE_UPDOWN,
		TYPE_GLRECT,
	};
private:
	// No private methods.
private:
	// No private variables.
};