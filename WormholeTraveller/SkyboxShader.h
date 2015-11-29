#pragma once
#include "ShaderProgram.h"
#include "prefdefs.h"

class SkyboxShader :
	public ShaderProgram
{
public:
	SkyboxShader();
	~SkyboxShader();

	virtual OpStatus init();
	GLuint getSamplerCube();

private:

	GLuint uSamplerCube;
};

