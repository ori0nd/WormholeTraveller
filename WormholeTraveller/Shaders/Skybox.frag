#version 420 core

in VS_OUT {
	vec3 texCoord;
} fs_in;

out vec4 color;

uniform samplerCube texSampler1;

void main ()
{
	color = texture(texSampler1, fs_in.texCoord);
}
