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

	uAmbient = glGetUniformLocation(getProgId(), "ambient");
	uSampler = glGetUniformLocation(getProgId(), "texSampler1");

	return OPS_OK;
}

void LightingShader::setAmbient(vec3 ambient)
{
	glUniform3f(uAmbient, ambient.r, ambient.g, ambient.b);
}

GLuint LightingShader::getTexSampler()
{
	return uSampler;
}

