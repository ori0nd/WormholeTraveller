#version 420 core

in VS_OUT {
	vec3 texCoord;
} fs_in;

out vec4 fragColor;

uniform samplerCube cubemapTexture;

void main()
{
	fragColor = texture(cubemapTexture, fs_in.texCoord);
}
