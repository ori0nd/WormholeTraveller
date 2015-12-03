#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

OpStatus Texture::loadTexture(const string& filename)
{
	texId = SOIL_load_OGL_texture
		(
			filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_DDS_LOAD_DIRECT
		);

	if (0 == texId)
	{
		cerr << "SOIL: Cannot load texture: " << filename << endl;
		return OPS_ERROR_LOAD_TEXTURE;
	}

	glBindTexture(GL_TEXTURE_2D, texId);

	// set repeat behaviour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set sampling behaviour
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::cout << filename << " is successfully loaded (id=" << texId << ")" << endl;

	return OPS_OK;
}

void Texture::bindToTextureUnit(int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void Texture::setTextureSampler(ShaderProgram & sh, const char * sampleName, GLuint samplerId)
{
	sh.useProgram(1);
	GLuint loc = glGetUniformLocation(sh.getProgId(), sampleName);
	glUniform1i(loc, samplerId);
}
