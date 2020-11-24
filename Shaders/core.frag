#version 330 core

in vec4 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform bool debugDraw;

void main()
{
	if (debugDraw)
	{
		FragColor = ourColor;
	}
	else
	{
		FragColor = texture(ourTexture, ourTexCoord) * ourColor;
	}
	//FragColor= vec4(ourColor);
	//FragColor.rg = ourTexCoord;
}


