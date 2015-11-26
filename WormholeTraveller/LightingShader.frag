#version 420 core

in VS_OUT {
	vec4 color;
	vec3 normal;
	vec3 position;
} fs_in;

out vec4 color;

uniform mat4 view;
uniform mat4 modelView;

uniform vec3 ambient;
uniform vec3 vLightDir;
uniform vec3 vDiffuseMaterial;
uniform vec3 vDiffuseLight;
uniform vec3 vSpecularMaterial;
uniform vec3 vSpecularLight;
uniform float fShininess;

void main()
{
	// Calculate view-space coordinate
	vec4 P = modelView * vec4(fs_in.position, 1.0);

	// Calculate normal in view space
	vec3 N = (mat3(modelView)) * fs_in.normal;

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

	color = fs_in.color * vec4((diffuse + specular + ambient), 1.0f);
}
