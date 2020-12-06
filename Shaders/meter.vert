/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			meter.vert
	Purpose:			Vertex shader for HUD meters which show fill level
						for properties such as fuel. Meter is quite similar
						to core shader.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/28/2020

- End Header --------------------------------------------------------*/

#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 texCoord;

out vec4 vertColor;
out vec2 vertTexCoord;

uniform mat4 modelTrans, viewTrans, viewProj;//, viewOffset;
uniform float uvScale;
uniform vec2 uvOffset;
uniform vec4 color;

void main()
{
	gl_Position = viewProj * viewTrans * modelTrans * vertex;
	vertColor = color;
	vertTexCoord = ( texCoord + uvOffset ) * uvScale;
}