#version 330 core

in vec4 vertColor;
in vec2 vertTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

uniform int col_type;
uniform float col_circleRadius;
uniform float col_aabbLeft, col_aabbRight, col_aabbTop, col_aabbBottom;
int COLLISION_NONE = 0;
int COLLISION_CICLE = 1;
int COLLISION_AABB = 2;

void main()
{
	// Render to fragColor based on texture.
	
	FragColor = texture(ourTexture, vertTexCoord) * vertColor;

	/*
	if (col_type == COLLISION_CICLE)
	{
		FragColor = vec4(vertTexCoord,0,1);
	}
	*/

	/*
	if (col_type == 0)
		FragColor += vec4(1,0,0,0);
	if (col_type == 1)
		FragColor += vec4(0,1,0,0);
	if (col_type ==2 )
		FragColor += vec4(0,0,1,0);
	*/
}


