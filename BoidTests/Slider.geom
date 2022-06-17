#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

in VS_OUT
{
	float length;
	float height;
	float slider;
} gs_in[];

out vec3 gColour;

uniform mat4 proj;

void main()
{

	float halfL = gs_in[0].length / 2.0;

	gColour = vec3(0.5, 0.5, 0.5);
	gl_Position = proj * (gl_in[0].gl_Position + vec4(-halfL, -gs_in[0].height, -1.0, 1.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(halfL, -gs_in[0].height, -1.0, 1.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(-halfL, gs_in[0].height, -1.0, 1.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(halfL, gs_in[0].height, -1.0, 1.0));
	EmitVertex();
	EndPrimitive();

	float slidePos = gs_in[0].slider - halfL;
	float slideSize = gs_in[0].height * 1.5;

	gColour = vec3(0.09, 0.57, 0.69);
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos - slideSize, -slideSize, -1.0, 1.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos + slideSize, -slideSize, -1.0, 1.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos - slideSize, slideSize, -1.0, 1.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos + slideSize, slideSize, -1.0, 1.0));
	EmitVertex();
	EndPrimitive();
}