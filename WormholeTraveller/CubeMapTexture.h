//
// Wormhole Traveller
//
// Cube Map Texture
//

#pragma once

#include <string>
#include <SOIL.h>

#include "ShaderProgram.h"
#include "Texture.h"

using namespace std;

class CubeMapTexture : public Texture
{
public:

	CubeMapTexture();
	~CubeMapTexture();

	virtual OpStatus loadTexture(const string& filename) { return OPS_NOK; }
	
	virtual OpStatus loadCubeMap(const string & posXFname,
		const string & negXFname,
		const string & posYFname,
		const string & negYFname,
		const string & posZFname,
		const string & negZFname);

	virtual void bindToTextureUnit(int textureUnit);
	virtual void CubeMapTexture::setTextureSampler(ShaderProgram& sh, const char * samplerName, GLuint samplerId);

private:

	string filenames[6];
	GLenum types[6];

};

