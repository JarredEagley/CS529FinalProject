// Part of following a modern openGL tutorial.
// https://www.youtube.com/watch?v=aA112viAx7c&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8&index=14

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color; 

out vec4 ourColor;

void main()
{
	gl_Position = vec4(position, 1.0);
	ourColor = color;	
}