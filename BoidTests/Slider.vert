#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aLength;
layout (location = 2) in float aHeight;
layout (location = 3) in float aSlider;


out VS_OUT
{
	float length;
	float height;
	float slider;
} vs_out;

void main()
{
	gl_Position = vec4(aPos, -1.0, 1.0);
	vs_out.length = aLength;
	vs_out.height = aHeight;
	vs_out.slider = aSlider;
}