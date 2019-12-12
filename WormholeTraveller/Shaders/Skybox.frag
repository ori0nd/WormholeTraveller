#version 330 core

in VS_OUT {
	vec3 texCoord;
} fs_in;

out vec4 color;

uniform samplerCube cubeSampler0;

void main ()
{
	color = texture(cubeSampler0, fs_in.texCoord);
}
