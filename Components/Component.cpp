/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Component.cpp
	Purpose:			Abstract GameObject class.
						This only implements the constructor, which sets
						owner to null and sets the type passed in.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/19/2020

- End Header --------------------------------------------------------*/

#include "Component.h"

Component::Component(unsigned int Type) : mType(Type), mpOwner(nullptr) // Set at the end of GameObject addcomponent function.
{
}