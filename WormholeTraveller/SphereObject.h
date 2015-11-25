//
// Wormhole Traveller
//
// SphereObject.h -- represents a in-game sphere (used for planets and wormholes)
//

#pragma once

#include "SceneObject.h"
#include "GeometryCommon.h"
#include "ShaderProgram.h"

#include <glm/vec4.hpp>

class SphereObject : public SceneObject
{
public:

	SphereObject(int numLong, int numLat);
	~SphereObject();

	void computeGeometry(std::vector<Vertex>& pVtc, std::vector<unsigned int>& pIndc) const;
	void renderObject(ShaderProgram& shader) const;

private:

	int longitudes, latitudes;

};

