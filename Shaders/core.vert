
#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor; 
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 ourTexCoord;

void main()
{
	gl_Position = aPosition;
	ourColor = aColor;
	ourTexCoord = aTexCoord;
}