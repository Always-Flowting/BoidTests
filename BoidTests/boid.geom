#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	float angle;
	float size;
	vec3 colour;
} gs_in[];

out vec3 gColour;

uniform mat4 proj;

mat2 rotation2d(float angle) {
	float s = sin(angle);
	float c = cos(angle);

	return mat2(c, s, -s, c);
}

void main()
{
	mat2 rot = rotation2d(gs_in[0].angle);
	float size = gs_in[0].size;

	gColour = gs_in[0].colour;

	gl_Position = proj * (gl_in[0].gl_Position + vec4(rot * vec2(-size * 2.0, size), 0.0, 0.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(rot * vec2(-size * 2.0, -size), 0.0, 0.0));
	EmitVertex();
	gl_Position = proj * (gl_in[0].gl_Position + vec4(rot * vec2(size * 2.0, 0.0), 0.0, 0.0));
	EmitVertex();
	EndPrimitive();
}