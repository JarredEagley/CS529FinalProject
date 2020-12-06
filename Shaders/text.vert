/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			text.vert
	Purpose:			vertex shader used for text rendering.
						This was created following the learnopengl.com
						text rendering tutorial.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		12/5/2020

- End Header --------------------------------------------------------*/

#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	texCoords = vertex.zw;
}