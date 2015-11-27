#version 420 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_OUT {
	vec3 normal;
	vec3 position; 
	vec2 texCoord;
} vs_out;

uniform mat4 modelView;
uniform mat4 projection;

void main(void)
{
	vs_out.normal = normal;
	vs_out.position = position.xyz;
	vs_out.texCoord = texCoord;

	mat4 modelViewProjection = projection * modelView;
	gl_Position = modelViewProjection * position;	
}
