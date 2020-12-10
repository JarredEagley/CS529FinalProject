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

uniform int drawType;
const int DRAW_TYPE_RECT = 0;
const int DRAW_TYPE_LINE = 1;
const int DRAW_TYPE_PHYS_AABB = 2;
const int DRAW_TYPE_PHYS_CIRCLE = 3;

void main()
{
	// Render to fragColor based on texture.
	switch (drawType)
	{
		case DRAW_TYPE_RECT:
			FragColor = texture(ourTexture, vertTexCoord) * vertColor;
			break;
		case DRAW_TYPE_LINE:
			break;

		case DRAW_TYPE_PHYS_AABB:
			FragColor = vec4(1,0,0,0.2); // Red transparent
			break;
		case DRAW_TYPE_PHYS_CIRCLE:
			vec2 uv = abs((vertTexCoord-0.5)*2);
			if (length(uv) < 1)
				FragColor = vec4(1, 0, 0, 0.2);
			else
				FragColor = vec4(0,0,1,0.05);
			break;

		default:
			FragColor = texture(ourTexture, vertTexCoord) * vertColor;
			break;
	}

}


