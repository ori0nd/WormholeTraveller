#version 420 core

layout (location = 0) in vec4 position;
layout (location = 2) in vec4 color;
layout (location = 1) in vec3 normal;

out VS_OUT {
	vec4 color;
	vec3 normal;
	vec3 position; 
} vs_out;

uniform mat4 modelView;
uniform mat4 projection;

void main(void) {
	vs_out.color = color;
	vs_out.normal = normal;
	vs_out.position = position.xyz;

	mat4 modelViewProjection = projection * modelView;
	gl_Position = modelViewProjection * position;	
}
