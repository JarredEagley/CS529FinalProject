/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			core.frag
	Purpose:			The core fragment shader used for most of the game.
						Things like ships, indicators, ect. use this. 
						Pretty much anything that is a GLRect, but not a meter.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/27/2020

- End Header --------------------------------------------------------*/

#version 330 core

in vec4 vertColor;
in vec2 vertTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

uniform int col_type;
uniform float col_circleRadius;
uniform float col_aabbLeft, col_aabbRight, col_aabbTop, col_aabbBottom;
int COLLISION_NONE = 0;
int COLLISION_CICLE = 1;
int COLLISION_AABB = 2;

void main()
{
	// Render to fragColor based on texture.
	
	FragColor = texture(ourTexture, vertTexCoord) * vertColor;

	/*
	if (col_type == COLLISION_CICLE)
	{
		FragColor = vec4(vertTexCoord,0,1);
	}
	*/
	

	/*
	if (col_type == 0)
		FragColor += vec4(1,0,0,0);
	if (col_type == 1)
		FragColor += vec4(0,1,0,0);
	if (col_type ==2 )
		FragColor += vec4(0,0,1,0);
	*/
}


