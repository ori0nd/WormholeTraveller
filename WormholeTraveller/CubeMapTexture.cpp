#ifdef _WIN32
#include "stdafx.h"
#endif

#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture()
{
}


CubeMapTexture::~CubeMapTexture()
{
}

OpStatus CubeMapTexture::loadCubeMap(
	const string & posXFname,
	const string & negXFname,
	const string & posYFname,
	const string & negYFname,
	const string & posZFname,
	const string & negZFname)
{
	filenames[0] = posXFname;
	filenames[1] = negXFname;
	filenames[2] = posYFname;
	filenames[3] = negYFname;
	filenames[4] = posZFname;
	filenames[5] = negZFname;

	types[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	types[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	types[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	types[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	types[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	types[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	unsigned char * image;
	int width, height;

	for (int i = 0; i < 6; ++i)
	{
		image = SOIL_load_image(
			filenames[i].c_str(),
			&width,
			&height,
			0,
			SOIL_LOAD_RGB);

		if (image == NULL)
		{
			cerr << "Cannot load cube map texture: " << filenames[i] << endl;
			return OPS_ERROR_LOAD_TEXTURE;
		}

		cout << "Loading texture to GPU: " << filenames[i] << "..." << endl;

		glTexImage2D(types[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		SOIL_free_image_data(image);
	}

	return OPS_OK;
}

void CubeMapTexture::bindToTextureUnit(int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
}

void CubeMapTexture::setTextureSampler(ShaderProgram& sh, const char * samplerName, GLuint samplerId)
{
	GLuint location = glGetUniformLocation(sh.getProgId(), samplerName);
	glUniform1i(location, samplerId);
}
