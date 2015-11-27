#include "stdafx.h"
#include "SkydomeShader.h"

SkydomeShader::SkydomeShader()
{
}

SkydomeShader::~SkydomeShader()
{
}

OpStatus SkydomeShader::init()
{
	OpStatus status;
	status = this->createShaderProgram(Preferences::getShaderPath("Skydome.vert").c_str(), Preferences::getShaderPath("Skydome.frag").c_str());
	if (OPS_FAILURE(status)) { return status; }
	return OPS_OK;
}

void SkydomeShader::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(textureLoc, textureUnit);
}
