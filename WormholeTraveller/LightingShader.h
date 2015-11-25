//
// Wormhole Traveller
//
// SphereObject.h -- represents a in-game sphere (used for planets and wormholes)
//

#pragma once

#include "ShaderProgram.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class LightingShader : public ShaderProgram
{
public:
	LightingShader();
	virtual ~LightingShader();

	OpStatus init();

private:

};

