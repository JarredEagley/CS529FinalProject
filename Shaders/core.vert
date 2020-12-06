#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertColor;
out vec2 vertTexCoord;

uniform mat4 modelTrans, viewTrans, viewProj;//, viewOffset;
uniform float uvScale;
uniform vec2 uvOffset;
uniform vec4 color;

uniform int col_type;
uniform float col_circleRadius;
uniform float col_aabbLeft, col_aabbRight, col_aabbTop, col_aabbBottom;
int COLLISION_NONE = 0;
int COLLISION_CICLE = 1;
int COLLISION_AABB = 2;

void main()
{

	gl_Position = viewProj * viewTrans * modelTrans * aPosition;
	vertColor = color;
	vertTexCoord = ( aTexCoord + uvOffset ) * uvScale;
}