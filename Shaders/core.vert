/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			core.vert
	Purpose:			The core vertex shader used for most of the game.
						Things like ships, indicators, ect. use this. 
						Pretty much anything that is a GLRect, but not a meter.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/27/2020

- End Header --------------------------------------------------------*/

#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 texCoord;

out vec4 vertColor;
out vec2 vertTexCoord;

uniform mat4 modelTrans, viewTrans, viewProj;//, viewOffset;

// GLRect
uniform float uvScale;
uniform vec2 uvOffset;
// GLRect and line
uniform vec4 color;

/*
uniform int drawType;
const int DRAW_TYPE_RECT = 0;
const int DRAW_TYPE_LINE = 1;
const int DRAW_TYPE_PHYS_AABB = 2;
const int DRAW_TYPE_PHYS_CIRCLE = 3;
*/

void main()
{
	gl_Position = viewProj * viewTrans * modelTrans * vertex;
	vertColor = color;
	vertTexCoord = ( texCoord + uvOffset ) * uvScale;
}