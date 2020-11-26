/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			FrameRateController.cpp
	Purpose:			Defines the class for the framerate controller.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone1
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/5/2020

- End Header --------------------------------------------------------*/

#pragma once

class FrameRateController
{
public:
	static FrameRateController* getInstance() 
	{
		if (!instance)
			instance = new FrameRateController;
		return instance;
	};
	void destroySingleton();

	void setMaxFramerate(unsigned int maxFrameRate);
	void frameStart();
	void frameEnd();
	unsigned int getFrameTime() { return mFrameTime; }
	float getFrameTimeSec() { return mFrameTime / 1000.0f; }

public:
private:
	FrameRateController(); // Constructor now just uses a default maxframerate of int max.
private:
	static FrameRateController *instance; // The signleton instance.
	unsigned int mTickStart;
	unsigned int mTickEnd;
	unsigned int mMaxFrameRate;
	unsigned int mTicksPerFrame;
	unsigned int mFrameTime;
};