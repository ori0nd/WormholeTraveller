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

	return OPS_OK;
}
