#pragma once

#include "ShaderProgram.h"
#include "prefdefs.h"

class SkydomeShader : public ShaderProgram
{
public:
	SkydomeShader();
	virtual ~SkydomeShader();

	OpStatus init();

	void setTextureUnit(unsigned int textureUnit);

private:

	GLuint textureLoc;

};

