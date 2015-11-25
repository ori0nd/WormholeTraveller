#include "stdafx.h"
#include "ShaderProgram.h"


ShaderProgram::ShaderProgram() : vShId(-1), fShId(-1), gShId(-1)
{
}


ShaderProgram::~ShaderProgram()
{
}

OpStatus ShaderProgram::createShaderProgram(char * vShFilename, char * fShFilename)
{
	OpStatus s;

	s = createShaderObject(vShFilename, GL_VERTEX_SHADER, &vShId);
	OpAssert(s, "Can't create vertex shader object");

	s = createShaderObject(fShFilename, GL_FRAGMENT_SHADER, &fShId);
	OpAssert(s, "Can't create fragment shader object");

	s = createShaderProgram(vShId, fShId, &programId);
	return s;
}

OpStatus ShaderProgram::createShaderProgram(char * vShFilename, char * fShFilename, char * gShFilename)
{
	fprintf(stderr, "!!! ShaderProgram::createShaderProgram() IS NOT IMPLEMENTED\n");
	return OPS_NOK;
}

void ShaderProgram::printShaderStatus()
{
	int rc;

	glGetProgramiv(programId, GL_VALIDATE_STATUS, &rc);
	if (rc == 0) {
		fprintf(stderr, "shader is valid status = %d \n", rc);
	}
	else {
		fprintf(stderr, "shader is not valid status = %d \n", rc);
	}

	rc = 0;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &rc);
	if (rc == 0) {
		fprintf(stderr, "shader program info log length= %d \n", rc);
	}
	else {
		char s[1024];
		int length = 0;
		s[0] = 0;
		glGetProgramInfoLog(programId, 1024, &length, s);
		fprintf(stderr, "shader info log = %s \n", s);
	}

	glGetProgramiv(programId, GL_ATTACHED_SHADERS, &rc);
	fprintf(stderr, "Number of attached shaders = %d \n", rc);

	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &rc);
	fprintf(stderr, "Number of active attributes = %d \n", rc);

	glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &rc);
	fprintf(stderr, "Max name length (active attriute) = %d \n", rc);

	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &rc);
	fprintf(stderr, "Number of active uniforms = %d \n", rc);

	glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &rc);
	fprintf(stderr, "Max name length (active uniform) = %d \n", rc);
}

void ShaderProgram::useProgram(int useProg)
{
	if (useProg != 0) {
		glUseProgram(programId);
	}
	else {
		glUseProgram(0);
	}
}

OpStatus ShaderProgram::copyMatrixToShader(const glm::mat4 matrix, const char * name)
{
	int uniformLoc = glGetUniformLocation(programId, name);
	
	if (uniformLoc == -1)
	{
		return OPS_UNIFORM_NOT_FOUND;
	}

	glUniformMatrix4fv(uniformLoc, 1, true, glm::value_ptr(matrix));
	return OPS_OK;
}

char * ShaderProgram::readCode(char * fname)
{
	char * shaderCode = NULL;
	int codeLength = 0;
	FILE *fp = NULL;

	// check for error in file name

	fopen_s(&fp, fname, "r");	// open file and check for errors
	if (fp == NULL) { return NULL; }

	// fine the length of code
	fseek(fp, 0L, SEEK_END);
	codeLength = ftell(fp);
	rewind(fp);		// could use fseek(fp, 0L, SEEK_SET)

	if (codeLength > 0) {
		// allocated space for code and check for errors
		shaderCode = (char *)malloc(codeLength + 1);	// add a space for the '\0'
		if (shaderCode == NULL) return(NULL);
		memset((void *)shaderCode, 0, codeLength + 1);
		fread((void *)shaderCode, sizeof(char), codeLength, fp);
		shaderCode[codeLength] = 0;
	}

	if (fp != NULL) fclose(fp);

	return shaderCode;
}

OpStatus ShaderProgram::createShaderObject(char * fname, int type, GLuint * shaderId)
{
	char *code = NULL;
	OpStatus status = OPS_OK;
	int rc = 0;


	// create a shader handle
	*shaderId = glCreateShader(type);

	// read the code
	code = readCode(fname);
	if (code == NULL) { return OPS_ERR_SHADER_READ; }

	// attach the source to the shaders
	glShaderSource(*shaderId, 1, &code, NULL);

	// compile the code
	glCompileShader(*shaderId);

	// check for errors
	glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &rc);
	if (rc != GL_TRUE) {
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr, "Error when creating a shader \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetShaderiv(*shaderId, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *)malloc(bufSize*sizeof(char) + 1);
		if (error != NULL) {
			glGetShaderInfoLog(*shaderId, bufSize, &length, error);
			fprintf(stderr, "%s \n", error);
		}
		if (error != NULL) free(error);
		status = OPS_ERR_SHADER_COMPILE;
	}
	else {
		status = OPS_OK;
	}

	if (code != NULL) free(code);	// free the source code of the shader

	return status;
}

OpStatus ShaderProgram::createShaderProgram(GLint vSh, GLint fSh, GLuint * progId)
{
	OpStatus status;
	int rc = 0;
	// get a handle to the shader program
	programId = glCreateProgram();
	// connect the shaders subprogram to the "main" program
	glAttachShader(programId, vSh);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error while attaching vertex shader\n");
		return OPS_ERR_SHADER_ATTACH;
	}

	glAttachShader(programId, fSh);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error while attaching fragment shader\n");
		return OPS_ERR_SHADER_ATTACH;
	}

	glLinkProgram(programId);

	// check for errors
	glGetProgramiv(programId, GL_LINK_STATUS, &rc);

	if (rc != GL_TRUE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr, "error while linking shader program\n");
		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *)malloc(bufSize*sizeof(char) + 1);
		if (error != NULL) {
			glGetProgramInfoLog(programId, bufSize, &length, error);
			fprintf(stderr, "%s \n", error);
		}
		if (error != NULL) free(error);
		status = OPS_ERR_SHADER_LINK;
	}
	else {
		status = OPS_OK;
	}

	printShaderStatus();  // may want to remove it.

	if (progId != NULL) *progId = programId;
	return status;
}

OpStatus ShaderProgram::createShaderProgram(GLint vSh, GLint fSh, GLint gSh, GLuint * progId)
{
	fprintf(stderr, "!!! ShaderProgram::createShaderProgram() IS NOT IMPLEMENTED\n");
	return OPS_NOK;
}
