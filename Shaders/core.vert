#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertColor;
out vec2 vertTexCoord;

uniform mat4 transform, viewTrans, viewProj;//, viewOffset;
uniform float uvScale;
uniform vec2 uvOffset;
uniform vec4 color;

void main()
{
	gl_Position = viewProj * viewTrans * transform * aPosition;
	vertColor = color;
	vertTexCoord = ( aTexCoord + uvOffset ) * uvScale;
}