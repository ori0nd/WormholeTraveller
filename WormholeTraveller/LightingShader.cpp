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

	status = this->createShaderProgram("LightingShader.vert", "LightingShader.frag");

	if (OPS_FAILURE(status)) { return status; }

	uAmbient = glGetUniformLocation(getProgId(), "ambient");

	return OPS_OK;
}

void LightingShader::setAmbient(vec3 ambient)
{
	glUniform3f(uAmbient, ambient.r, ambient.g, ambient.b);
}

