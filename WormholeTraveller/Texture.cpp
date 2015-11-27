#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

OpStatus Texture::loadTextures(const string& filename, GLuint texTarget)
{
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	unsigned char * image;
	int width, height;

	// load the image
	image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	if (image == NULL) { 
		cerr << "SOIL: Cannot load texture: " << filename << endl;
		return OPS_ERROR_LOAD_TEXTURE; 
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);
	image = NULL;

	// set repeat behaviour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set sampling behaviour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return OPS_OK;
}

void Texture::bindToTextureUnit(int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void Texture::setTextureSampler(ShaderProgram & sh, const char * sampleName, GLuint samplerId)
{
	GLuint loc = glGetUniformLocation(sh.getProgId(), sampleName);
	glUniform1i(loc, samplerId);
}
