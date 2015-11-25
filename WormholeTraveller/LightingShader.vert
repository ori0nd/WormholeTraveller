#version 420 core

layout (location = 0) in vec4 position;
layout (location = 2) in vec4 color;
layout (location = 1) in vec3 normal;

out VS_OUT {
	vec3 color;
} vs_out;

uniform mat4 modelView;
uniform mat4 projection;

uniform vec3 ambient;

void main(void)
{
	mat4 modelViewProjection = projection * modelView;
	vs_out.color = color.xyz * ambient;
	gl_Position = modelViewProjection * position;
}
