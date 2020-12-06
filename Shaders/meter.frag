/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			meter.frag
	Purpose:			Fragment shader for HUD meters which show fill level
						for properties such as fuel. Meter is quite similar
						to core shader.
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_FinalProject
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		11/28/2020

- End Header --------------------------------------------------------*/

#version 330 core

in vec4 vertColor; // No longer actually from vetex data...
in vec2 vertTexCoord;

out vec4 FragColor;

uniform float fillLevel;
uniform vec4 secondaryColor;
uniform sampler2D ourTexture;

void main()
{
	// Render to fragColor based on texture.
	vec4 currentColor = (fillLevel/100) * vertColor;
	currentColor += (1-(fillLevel/100) )* secondaryColor;

	FragColor = texture(ourTexture, vertTexCoord) * currentColor;
	if (vertTexCoord.y > (fillLevel/100))
	{
		FragColor.a /= 2;
	}
}


