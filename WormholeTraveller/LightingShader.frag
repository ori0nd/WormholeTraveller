#version 330 core

in vec4 fragColor;
in vec4 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

struct DirectionalLight {
	vec3 color;
	vec3 direction;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	float specularPower;
};

uniform mat4 model;
uniform DirectionalLight light;
uniform vec4 eyePosition;

void main()
{
	// Simple ambient color. Uniform intensity and color across all vrtcs
	vec4 ambient = vec4(light.color, 1.0f) * light.ambientIntensity;

	// Compute cosine between the light direction and the normal.
	// Must normalize it, since vertex shader may have interpolated it,
	// and the normal may not be unit length anymore.
	// Note: light direction is not normalized

	mat3 normalMtx = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMtx * fragNormal.xyz);

	vec4 diffuse;
	vec4 specular;

	float diffuseFactor = dot(normal, -light.direction);
	
	if (diffuseFactor > 0) {
		// Notice, when light is coming at 90* angle, we have cos=1 and maximum intensity.
		diffuse = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	} 
	else {
		// The light is coming at an obtuse angle -> can't reach the fragment 
		diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	vec3 toViewport = normalize(eyePosition.xyz - fragPosition);
	vec3 reflection = normalize(reflect(light.direction, normal));
    float specularFactor = pow(dot(toViewport, reflection), light.specularPower);
    
	if (specularFactor > 0) {
        specular = vec4(light.color, 1.0f) * light.specularIntensity * specularFactor; 
	} 
	else {
		specular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	outColor = fragColor * (ambient + diffuse + specular);
}
