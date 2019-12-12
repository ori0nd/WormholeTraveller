//
// Wormhole Traveller
//
// Texture
//

#pragma once

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <string>

#include "ShaderProgram.h"

using namespace std;

class Texture
{
public:

	Texture();
	virtual ~Texture();

	virtual OpStatus loadTexture(const string& filename);
	virtual void bindToTextureUnit(int textureUnit);
	virtual void setTextureSampler(ShaderProgram& sh, const char * sampleName, GLuint samplerId);

protected:

	GLuint texId; // texture handle
	int textureUnit; // tex. unit for the sampler

};
