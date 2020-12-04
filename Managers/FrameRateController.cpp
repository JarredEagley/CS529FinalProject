/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FrameRateController.cpp
	Purpose:			Implements class logic for the FrameRateController.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/5/2020

- End Header --------------------------------------------------------*/

#include "FrameRatecontroller.h"
#include "limits.h"
#include "SDL_timer.h"

FrameRateController* FrameRateController::instance = nullptr;

void FrameRateController::destroySingleton()
{
	delete instance;
}
// End of singleton stuff //

FrameRateController::FrameRateController()
{
	// Set the max framerate to max unsigned integer.
	this->setMaxFramerate(UINT_MAX);
	// Set other members.
	mTickStart = 0;
	mTickEnd = 0;
	mFrameTime = 0;
}

void FrameRateController::setMaxFramerate(unsigned int maxFrameRate)
{
	mMaxFrameRate = maxFrameRate;
	// Set ticks per frame.
	mTicksPerFrame = 1000 / mMaxFrameRate;
}

void FrameRateController::setMaxFrameTime(unsigned int maxFrameTime)
{
	mMaxFrameTime = maxFrameTime;
}

void FrameRateController::frameStart()
{
	mTickStart = SDL_GetTicks();
}

void FrameRateController::frameEnd()
{
	mTickEnd = SDL_GetTicks();
	while ((mTickEnd - mTickStart) < mTicksPerFrame)
		mTickEnd = SDL_GetTicks();
	
	mFrameTime = mTickEnd - mTickStart;
}


unsigned int FrameRateController::getFrameTime()
{ 
	if (mFrameTime > mMaxFrameTime)
		return mMaxFrameTime;
	return mFrameTime;
}

float FrameRateController::getFrameTimeSec()
{ 
	return ((float)getFrameTime() / 1000.0f);
}