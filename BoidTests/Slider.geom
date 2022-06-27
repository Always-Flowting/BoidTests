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

	gColour = vec3(0.35, 0.35, 0.35);
	gl_Position = proj * (gl_in[0].gl_Position + vec4(-halfL, -gs_in[0].height, 0.0, 0.0));
	EmitVertex();

	gl_Position = proj * (gl_in[0].gl_Position + vec4(halfL, -gs_in[0].height, 0.0, 0.0));
	EmitVertex();

	gl_Position = proj * (gl_in[0].gl_Position + vec4(-halfL, gs_in[0].height, 0.0, 0.0));
	EmitVertex();

	gl_Position = proj * (gl_in[0].gl_Position + vec4(halfL, gs_in[0].height, 0.0, 0.0));
	EmitVertex();

	EndPrimitive();

	float slidePos = (gs_in[0].slider * gs_in[0].length) - halfL;
	float slideSize = gs_in[0].height * 1.5;

	gColour = vec3(0.75, 0.75, 0.78);
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos - slideSize, -slideSize, 0.0, 0.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos + slideSize, -slideSize, 0.0, 0.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos - slideSize, slideSize, 0.0, 0.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(slidePos + slideSize, slideSize, 0.0, 0.0));
	EmitVertex();
	EndPrimitive();
}