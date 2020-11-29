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


