#version 400

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInformation {
	vec4 position; // in eye coordinates
	vec3 ambientIntensity;
	vec3 diffuseIntensity;
	vec3 specularIntensity;
};

uniform LightInformation light;

struct MaterialInformation {
	vec3 ambientReflectivity;
	vec3 diffuseReflectivity;
	vec3 specularReflectivity;
	float shininess;
};

uniform MaterialInformation material;cs
uniform mat4 model;
uniform mat4 modelView;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
	//mat3 normalMatrix = transpose(inverse(mat3(modelView)));
	vec3 tnorm = normalize(normalMatrix * VertexNormal);
	vec4 eyeCoords = modelView * VertexPosition;

	vec3 s = normalize(vec3(light.position));
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);
	vec3 ambient = light.ambientIntensity * material.ambientReflectivity;

	float sDotN = max(dot(s, tnorm), 0.0);
	vec3 diffuse = light.diffuseIntensity * material.diffuseReflectivity * sDotN;
	
	vec3 spec = vec3(0.0);
	if (sDotN > 0.0)
	{
		spec = light.specularIntensity * material.specularReflectivity * 
			pow(max(dot(r, v), 0.0), material.shininess);
	}
	
	LightIntensity = ambient + diffuse + spec; 

	mat4 modelViewProjection = projection * modelView;
	gl_Position = modelViewProjection * VertexPosition;
}
