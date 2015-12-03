#version 420 core

in VS_OUT {
	vec3 normal;
	vec3 position;
	vec2 texCoord;
	vec3 reflectionVec;
} fs_in;

out vec4 color;

uniform samplerCube cubeMap;
uniform sampler2D decalMap;
uniform float reflectivity;

void main()
{
	vec4 reflectedColor = texture(cubeMap, fs_in.reflectionVec);
	vec4 decalColor = texture(decalMap, fs_in.texCoord);

	color = mix(decalColor, reflectedColor, reflectivity);

}
