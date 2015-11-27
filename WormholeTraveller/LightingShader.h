//
// Wormhole Traveller
//
// SphereObject.h -- represents a in-game sphere (used for planets and wormholes)
//

#pragma once

#include "ShaderProgram.h"
#include "prefdefs.h"

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace std;
using namespace glm;

typedef struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

typedef struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

class LightingShader : public ShaderProgram
{
public:
	LightingShader();
	virtual ~LightingShader();

	OpStatus init();

	void setLight(const DirectionalLight& light);
	void setMaterial(const Material& material);
	GLuint getTexSampler();

private:

	GLuint uDirection;
	GLuint uAmbient;
	GLuint uDiffuseLight;
	GLuint uSpecularLight;

	GLuint uDiffuseMaterial;
	GLuint uSpecularMaterial;
	GLuint uShininess;

	GLuint uSampler;

};

