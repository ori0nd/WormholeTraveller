#ifdef _WIN32
#include "stdafx.h"
#endif

#include "WormholeShader.h"

WormholeShader::WormholeShader()
{
}


WormholeShader::~WormholeShader()
{
}

OpStatus WormholeShader::init()
{
	OpStatus status;
	int rc = glGetError();
	status = this->createShaderProgram(Preferences::getShaderPath("Wormhole.vert").c_str(), Preferences::getShaderPath("Wormhole.frag").c_str());
	if (OPS_FAILURE(status)) { return status; }
	return OPS_OK;
}

GLuint WormholeShader::getTexSampler()
{
	return uSampler;
}
