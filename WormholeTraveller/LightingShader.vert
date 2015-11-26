#version 420 core

layout (location = 0) in vec4 position;
layout (location = 2) in vec4 color;
layout (location = 1) in vec3 normal;

out VS_OUT {
	vec3 color;
} vs_out;

uniform mat4 view;
uniform mat4 modelView;
uniform mat4 projection;

uniform vec3 ambient;
uniform vec3 vLightDir;
uniform vec3 vDiffuseMaterial;
uniform vec3 vDiffuseLight;
uniform vec3 vSpecularMaterial;
uniform vec3 vSpecularLight;
uniform float fShininess;

void main(void)
{
	// Calculate view-space coordinate
	vec4 P = modelView * position;

	// Calculate normal in view space
	vec3 N = (mat3(modelView)) * normal;

	// Calculate view-space light vector
	vec3 L = mat3(view) * -vLightDir;

	// Calculate view vector (negative of view-space vtx position)
	vec3 V = -P.xyz;

	// Normalize all three
	N = normalize(N);
	L = normalize(L);
	V = normalize(V);

	// Calculate R by reflecting L around plane def. by N
	vec3 R = reflect(-L, N);

	// Calculate diffuse and specular factors
	
	// Compute cos between the normal and the light direction, avoiding negatives
	vec3 diffuse = max(0.0, dot(N, L)) * vDiffuseLight * vDiffuseMaterial;
	vec3 specular = pow(max(dot(R, V), 0.0), fShininess) * vSpecularLight * vSpecularMaterial;

	vs_out.color = color.xyz
	 * ( ambient + specular + diffuse );

	mat4 modelViewProjection = projection * modelView;
	gl_Position = modelViewProjection * position;
	
}
