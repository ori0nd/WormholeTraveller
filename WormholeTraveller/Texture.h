//
// Wormhole Traveller
//
// Texture
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <SOIL.h>
#include <iostream>
#include <string>

#include "ShaderProgram.h"

using namespace std;

class Texture
{
public:

	Texture();
	virtual ~Texture();

	OpStatus loadTextures(const string& filename, GLuint texTarget);
	void bindToTextureUnit(int textureUnit);
	void setTextureSampler(ShaderProgram& sh, const char * sampleName, GLuint samplerId);

private:

	GLuint texId; // texture handle
	int textureUnit; // tex. unit for the sampler

};

#endif /* TEXTURE_H */
