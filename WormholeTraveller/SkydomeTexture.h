//
// Wormhole Traveller
//
// SkydomeTexture 
//


#pragma once

#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <string>
#include <SOIL.h>

using namespace std;

class SkydomeTexture
{
public:
	
	SkydomeTexture(
		const string& posXFname,	
		const string& negXFname,
		const string& posYFname,
		const string& negYFname,
		const string& posZFname,
		const string& negZFname
	);

	~SkydomeTexture();

	OpStatus load();
	void bind(GLenum textureUnit);

private:

	string filenames[6];
	GLuint textureObject;

	GLenum types[6];
};

