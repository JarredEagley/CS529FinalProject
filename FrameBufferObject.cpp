/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FrameBufferObject.cpp
	Purpose:			Implementations for FrameBufferObject.h
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/28/2020

- End Header --------------------------------------------------------*/

#include "FrameBufferObject.h"
#include <iostream>


void FrameBufferObject::createFBO(int const width, int const height)
{
	// Generate the openGL FBO.
	mWidth = width;
	mHeight = height;
	glGenFramebuffers(1, &mFboId);
	glBindFramebuffer(GL_FRAMEBUFFER, mFboId);

	// No need to store a depth buffer for my application.
	// Create a texture, attach the FBO's color. 
	glGenTextures(1, &mTexId);
	glBindTexture(GL_TEXTURE_2D, mTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// Going to make FBO texture not wrap, and have linear filter.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// The magic function which attaches the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);

	// Make sure the binding didn't fail.
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBufferObject encountered an error: " << status << std::endl;
	}

	// Unbind until this FBO is needed.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBufferObject::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
}

void FrameBufferObject::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
