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
	Project:			JarredEagley_FinalProject
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
		if (strcmp(str, "TYPE_CONTROLLERSLIDER") == 0)	return TYPE_CONTROLLERSLIDER;
		if (strcmp(str, "TYPE_CONTROLLERSHIP") == 0)	return TYPE_CONTROLLERSHIP;
		if (strcmp(str, "TYPE_GLRECT") == 0)			return TYPE_GLRECT;
		if (strcmp(str, "TYPE_GLTEXT") == 0)			return TYPE_GLTEXT;
		if (strcmp(str, "TYPE_CAMERA") == 0)			return TYPE_CAMERA;
		if (strcmp(str, "TYPE_BACKGROUNDGRID") == 0)	return TYPE_BACKGROUNDGRID;
		if (strcmp(str, "TYPE_PHYSICSBODY") == 0)		return TYPE_PHYSICSBODY;
		if (strcmp(str, "TYPE_DIRINDICATOR") == 0)		return TYPE_DIRINDICATOR;
		if (strcmp(str, "TYPE_SHIPDATA") == 0)			return TYPE_SHIPDATA;
		if (strcmp(str, "TYPE_TURRET") == 0)			return TYPE_TURRET;
		if (strcmp(str, "TYPE_METER_FUEL") == 0)		return TYPE_METER_FUEL;
		if (strcmp(str, "TYPE_METER_HEALTH") == 0)		return TYPE_METER_HEALTH;
		if (strcmp(str, "TYPE_MARKER_THROTTLE") == 0)	return TYPE_MARKER_THROTTLE;
		if (strcmp(str, "TYPE_FOLLOWCURSOR") == 0)		return TYPE_FOLLOWCURSOR;
		if (strcmp(str, "TYPE_EXPLOSION") == 0)			return TYPE_EXPLOSION;
		
		
		return TYPE_UNDEFINED;
	}
public:
	// Types of component.
	static enum COMPONENT_TYPE
	{
		TYPE_UNDEFINED,
		TYPE_TRANSFORM,
		TYPE_TIMER,
		TYPE_CONTROLLERSLIDER,
		TYPE_CONTROLLERSHIP,
		TYPE_GLRECT,
		TYPE_GLTEXT,
		TYPE_CAMERA,
		TYPE_BACKGROUNDGRID,
		TYPE_PHYSICSBODY,
		TYPE_DIRINDICATOR,
		TYPE_SHIPDATA,
		TYPE_TURRET,
		TYPE_METER_FUEL,
		TYPE_METER_HEALTH,
		TYPE_MARKER_THROTTLE,
		TYPE_FOLLOWCURSOR,
		TYPE_EXPLOSION
	};
private:
	// No private methods.
private:
	// No private variables.
};