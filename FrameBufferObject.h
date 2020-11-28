/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FrameBufferObject.h
	Purpose:			Based on (but not copied from) the encapsulation
						provided in CS 541. This encpsulates Frame Buffer
						Objects, allowing for multiple render passes to
						be done more easily.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/28/2020

- End Header --------------------------------------------------------*/

#pragma once
#include "GL/glew.h"

class FrameBufferObject
{
public:
	void createFBO(int const width, int const height);
	void bindFBO();
	void unbindFBO();

public:
	// The ID of this FBO.
	GLuint mFboId;
	// The ID of the texture we're rendering to.
	GLuint mTexId;
	// Width and height of the texture.
	int mWidth, mHeight;
private:
	// No private methods.
private:
	// No private variables.
};

