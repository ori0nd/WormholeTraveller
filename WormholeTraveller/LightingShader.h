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
	
	glm::vec3 color;
	glm::vec3 direction;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
	float specularPower;

} DirectionalLight;

class LightingShader : public ShaderProgram
{
public:
	LightingShader();
	virtual ~LightingShader();

	OpStatus init();

	void setLight(const DirectionalLight light);
	void setEyePosition(const glm::vec4 pos);

private:

	// Uniform locations
	GLuint lightColorLoc;
	GLuint lightAmbIntensityLoc;
	GLuint lightDiffuseIntensityLoc;
	GLuint lightDirectionLoc;
	GLuint lightSpecularIntensityLoc;
	GLuint lightSpecularPowerLoc;
	GLuint eyePositionLoc;
};

