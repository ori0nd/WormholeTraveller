//
// Wormhole Traveller
//
// SphereObject.h -- represents a in-game sphere (used for planets and wormholes)
//

#pragma once

#include "ShaderProgram.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef struct DirectionalLight {
	
	glm::vec4 position;
	glm::vec3 ambientInt;
	glm::vec3 diffuseInt;
	glm::vec3 specularInt;

} DirectionalLight;

typedef struct Material {

	glm::vec3 ambientReflect;
	glm::vec3 diffuseReflect;
	glm::vec3 specularReflect;
	float shininess;

} Material;

class LightingShader : public ShaderProgram
{
public:
	LightingShader();
	virtual ~LightingShader();

	OpStatus init();

	void setLight(const DirectionalLight light);
	void setMaterial(const Material material);

private:

	// Uniform locations
	GLuint ulLightPosition;
	GLuint ulLightAmbientInt;
	GLuint ulLightDiffuseInt;
	GLuint ulLightSpecularInt;

	GLuint ulMatAmbientReflect;
	GLuint ulMatDiffuseReflect;
	GLuint ulMatSpecularReflect;
	GLuint ulMatShininess;
};

