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

uniform vec3 ambient = vec3(0.2, 0.2, 0.2);

uniform vec3 vLightDir = vec3(50, 0, 50);
//uniform vec3 vDiffuseMaterial = vec3(0.0);
uniform vec3 vDiffuseLight = vec3(0.6, 0.3, 0.4);
//uniform vec3 vSpecularMaterial = vec3(0.0);
uniform vec3 vSpecularLight = vec3(0.87, 0.93, 0.73);
uniform float fShininess = 16.0;

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
	vec3 diffuse = max(0.0, dot(N, L)) * vDiffuseLight;
	vec3 specular = pow(max(dot(R, V), 0.0), fShininess) * vSpecularLight;

	vs_out.color = color.xyz
	 * ( ambient + specular + diffuse );

	mat4 modelViewProjection = projection * modelView;
	gl_Position = modelViewProjection * position;
	
}
