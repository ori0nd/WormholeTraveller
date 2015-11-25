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

	ulLightAmbient = glGetUniformLocation(getProgId(), "Ambient");

	return OPS_OK;
}

void LightingShader::setAmbientLight(glm::vec4 intensityRgb)
{
	glUniform4f(ulLightAmbient, intensityRgb.r, intensityRgb.g, intensityRgb.b, intensityRgb.a);
}

