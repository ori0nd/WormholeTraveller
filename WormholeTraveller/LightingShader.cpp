#include "stdafx.h"
#include "LightingShader.h"


LightingShader::LightingShader()
{
}


LightingShader::~LightingShader()
{
}

OpStatus LightingShader::init()
{
	OpStatus status;
	
	status = this->createShaderProgram("LightingShader.vert", "LightingShader.frag");
	
	if (OPS_FAILURE(status)) { return status; }

	lightColorLoc = glGetUniformLocation(getProgId(), "light.color");
	lightAmbIntensityLoc = glGetUniformLocation(getProgId(), "light.ambientIntensity");
	lightDiffuseIntensityLoc = glGetUniformLocation(getProgId(), "light.diffuseIntensity");
	lightSpecularIntensityLoc = glGetUniformLocation(getProgId(), "light.specularIntensity");
	lightSpecularPowerLoc = glGetUniformLocation(getProgId(), "light.specularPower");
	lightDirectionLoc = glGetUniformLocation(getProgId(), "light.direction");
	eyePositionLoc = glGetUniformLocation(getProgId(), "eyePosition");

	return OPS_OK;
}

void LightingShader::setLight(const DirectionalLight light)
{
	glUniform3f(this->lightColorLoc, light.color.x, light.color.y, light.color.z);
	glUniform1f(this->lightAmbIntensityLoc, light.ambientIntensity);
	glm::vec3 direction = glm::normalize(light.direction);
	glUniform3f(this->lightDirectionLoc, direction.x, direction.y, direction.z);
	glUniform1f(this->lightDiffuseIntensityLoc, light.diffuseIntensity);
	glUniform1f(this->lightSpecularIntensityLoc, light.specularIntensity);
	glUniform1f(this->lightSpecularPowerLoc, light.specularPower);
}

void LightingShader::setEyePosition(const glm::vec4 pos)
{
	glUniform4f(this->eyePositionLoc, pos.x, pos.y, pos.z, pos.w);
}
