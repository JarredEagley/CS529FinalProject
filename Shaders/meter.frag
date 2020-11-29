#version 330 core

in vec4 vertColor;
in vec2 vertTexCoord;

out vec4 FragColor;

uniform float fillLevel;
uniform sampler2D ourTexture;

void main()
{
	// Render to fragColor based on texture.
	FragColor = texture(ourTexture, vertTexCoord) * vertColor;
	if (vertTexCoord.y > (fillLevel/100.0))
	{
		FragColor.a /= 2;
	}
}


