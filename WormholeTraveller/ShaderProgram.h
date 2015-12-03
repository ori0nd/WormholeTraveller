//
// Wormhole Traveller
//
// ShaderProgram.h -- a wrapper around GLSL shader programs
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "glew.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

class ShaderProgram
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	GLuint getProgId() const { return programId; }

	OpStatus createShaderProgram(const char * vShFilename, const char * fShFilename);
	//OpStatus createShaderProgram(char * vShFilename, char * fShFilename, char * gShFilename);

	void printShaderStatus();
	void useProgram(int useProg);

	OpStatus copyMatrixToShader(const glm::mat4 matrix, const char* name);
	OpStatus copyVectorToShader(const glm::vec3 vector, const char* name);
	OpStatus copyVectorToShader(const glm::vec4 vector, const char* name);
	OpStatus copyIntToShader(const int value, const char* name);

	virtual OpStatus init() = 0;

private:

	GLuint programId;
	GLuint vShId, fShId, gShId;

	char * readCode(const char * fname);

	OpStatus createShaderObject(const char* fname, int type, GLuint * shaderId);
	OpStatus createShaderProgram(GLint vSh, GLint fSh, GLuint * progId);
	//OpStatus createShaderProgram(GLint vSh, GLint fSh, GLint gSh, GLuint * progId);
};

