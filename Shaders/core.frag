#version 330 core

in vec4 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor= vec4(ourColor);
	//FragColor = texture(ourTexture, ourTexCoord) * ourColor;
	//FragColor.rg = ourTexCoord;
}


