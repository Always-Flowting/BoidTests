#version 330 core

layout (location = 0) in float aPosX;
layout (location = 1) in float aPosY;
layout (location = 2) in float aLength;
layout (location = 3) in float aHeight;
layout (location = 4) in float aSlider;


out VS_OUT
{
	float length;
	float height;
	float slider;
} vs_out;

void main()
{
	gl_Position = vec4(aPosX, aPosY, -1.0, 1.0);
	vs_out.length = aLength;
	vs_out.height = aHeight;
	vs_out.slider = aSlider;
}