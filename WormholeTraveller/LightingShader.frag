#version 330 core

in vec4 Color;

out vec4 FragColor;

uniform vec4 Ambient;

void main()
{
	vec3 scatteredLight = vec3(Ambient);
	vec3 rgb = min(Color.rgb * scatteredLight, vec3(1.0));
	FragColor = vec4(rgb, Color.a);
}