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

uniform vec3 vLightDir;
uniform vec3 vDiffuseMaterial;
uniform vec3 vDiffuseLight;
//uniform vec3 vSpecularMaterial;
//uniform vec3 vSpecularLight;

void main(void)
{
	mat4 modelViewProjection = projection * modelView;

	// Compute cos between the normal and the light direction, avoiding negatives
	float fDotProduct = max(0.0, dot(normal, normalize(-vLightDir)));
	vec3 vDiffuseColor = vDiffuseMaterial * fDotProduct * vDiffuseLight;
	vs_out.color = color.xyz * (ambient + vDiffuseColor);
	gl_Position = modelViewProjection * position;
}
