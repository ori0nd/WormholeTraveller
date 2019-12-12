#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

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
	vs_out.position = position;
	vs_out.texCoord = uv;

	mat4 modelViewProjection = projection * modelView;
	gl_Position = modelViewProjection * vec4(position, 1.0);	
}
