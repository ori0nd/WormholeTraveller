//
// Wormhole Traveller
//
// ShaderProgram.h -- a wrapper around GLSL shader programs
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "glew.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

class ShaderProgram
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	GLuint getProgId() const { return programId; }

	OpStatus createShaderProgram(char * vShFilename, char * fShFilename);
	OpStatus createShaderProgram(char * vShFilename, char * fShFilename, char * gShFilename);

	void printShaderStatus();
	void useProgram(int useProg);
	OpStatus copyMatrixToShader(const glm::mat3 matrix, const char * name);
	OpStatus copyMatrixToShader(const glm::mat4 matrix, const char* name);

	virtual OpStatus init() = 0;

private:

	GLuint programId;
	GLuint vShId, fShId, gShId;

	char * readCode(char * fname);

	OpStatus createShaderObject(char* fname, int type, GLuint * shaderId);
	OpStatus createShaderProgram(GLint vSh, GLint fSh, GLuint * progId);
	OpStatus createShaderProgram(GLint vSh, GLint fSh, GLint gSh, GLuint * progId);
};

