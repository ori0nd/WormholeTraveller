#pragma once

#include "ShaderProgram.h"
#include "prefdefs.h"

class WormholeShader :
	public ShaderProgram
{
public:

	WormholeShader();
	virtual ~WormholeShader();

	OpStatus init();

	GLuint getTexSampler();

private:

	GLuint uSampler;

};

