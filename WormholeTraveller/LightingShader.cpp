#include "stdafx.h"
#include "LightingShader.h"

using namespace glm;

LightingShader::LightingShader()
{
}


LightingShader::~LightingShader()
{
}

OpStatus LightingShader::init()
{
	OpStatus status;

	status = this->createShaderProgram(Preferences::getShaderPath("LightingShader.vert").c_str(), Preferences::getShaderPath("LightingShader.frag").c_str());

	if (OPS_FAILURE(status)) { return status; }

	uAmbient = glGetUniformLocation(getProgId(), "light.ambient");
	uSampler = glGetUniformLocation(getProgId(), "texSampler1");
	uDirection = glGetUniformLocation(getProgId(), "light.direction");
	uDiffuseLight = glGetUniformLocation(getProgId(), "light.diffuse");
	uSpecularLight = glGetUniformLocation(getProgId(), "light.specular");
	uDiffuseMaterial = glGetUniformLocation(getProgId(), "material.diffuse");
	uSpecularMaterial = glGetUniformLocation(getProgId(), "material.specular");
	uShininess = glGetUniformLocation(getProgId(), "material.shininess");

	return OPS_OK;
}

void LightingShader::setLight(const DirectionalLight & light)
{
	glUniform3f(uDirection, light.direction.x, light.direction.y, light.direction.z);
	glUniform3f(uAmbient, light.ambient.r, light.ambient.g, light.ambient.b);
	glUniform3f(uDiffuseLight, light.diffuse.r, light.diffuse.g, light.diffuse.b);
	glUniform3f(uSpecularLight, light.specular.r, light.specular.g, light.specular.b);
}

void LightingShader::setMaterial(const Material & material)
{
	glUniform3f(uDiffuseMaterial, material.diffuse.r, material.diffuse.g, material.diffuse.b);
	glUniform3f(uSpecularMaterial, material.specular.r, material.specular.g, material.specular.b);
	glUniform1f(uShininess, material.shininess);
}

GLuint LightingShader::getTexSampler()
{
	return uSampler;
}

