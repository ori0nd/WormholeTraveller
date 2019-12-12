#version 330 core

layout (location = 0) in vec3 position;

out VS_OUT {
	vec3 texCoord;
} vs_out;

// NOTE: view matrix must not have any translation. (only rotations)
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	vs_out.texCoord = position;
	gl_Position = projection * view * vec4(position, 1.0);
}
