#version 420 core

in VS_OUT 
{
	vec3 normal;
	vec3 position;
	vec2 texCoord;
} fs_in;

out vec4 color;

uniform mat4 view;
uniform mat4 modelView;

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform sampler2D texSampler1;

uniform DirectionalLight light;
uniform Material material;

//uniform vec3 vLightDir = vec3(-100, 25, 15);
//uniform vec3 vDiffuseMaterial;
//uniform vec3 vDiffuseLight;
//uniform vec3 vSpecularMaterial;
//uniform vec3 vSpecularLight;
//uniform float fShininess = 4;

void main()
{
	// Calculate view-space coordinate
	vec4 P = modelView * vec4(fs_in.position, 1.0);

	// Calculate normal in view space
	vec3 N = (mat3(modelView)) * fs_in.normal;

	// Calculate view-space light vector
	vec3 L = mat3(view) * - light.direction;

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
	vec3 diffuse = max(0.0, dot(N, L)) * light.diffuse * material.diffuse;
	vec3 specular = pow(max(dot(R, V), 0.0), material.shininess) * light.specular * material.specular;

	color = texture2D(texSampler1, fs_in.texCoord) * vec4((diffuse + specular + light.ambient), 1.0f);
}
