#ifdef _WIN32
#include "stdafx.h"
#endif

#include "SkyboxShader.h"

SkyboxShader::SkyboxShader()
{
}

SkyboxShader::~SkyboxShader()
{
}

OpStatus SkyboxShader::init()
{
	OpStatus status;
	status = this->createShaderProgram(Preferences::getShaderPath("Skybox.vert").c_str(), Preferences::getShaderPath("Skybox.frag").c_str());
	if (OPS_FAILURE(status)) { return status; }
	uSamplerCube = glGetUniformLocation(getProgId(), "texSampler1");
	return OPS_OK;
}

GLuint SkyboxShader::getSamplerCube()
{
	return uSamplerCube;
}
