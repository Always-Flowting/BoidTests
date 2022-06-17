#version 330 core

in vec3 gColour;

out vec4 FragColour;

void main()
{
	FragColour = vec4(gColour, 1.0);
}