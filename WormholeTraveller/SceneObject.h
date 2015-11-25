//
// Wormhole Traveller
//
// SceneObject.h -- class representing a single object in the game world (e.g. a car or character)
//

#pragma once

#include <vector>
#include <stddef.h>

#include <glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "GeometryCommon.h"

class SceneObject
{
public:

	SceneObject();
	virtual ~SceneObject();

	OpStatus createVao(ShaderProgram& sh, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	virtual void getModelTransform(glm::mat4 * dest) const;
	virtual void renderObject(ShaderProgram& sh) const;

	virtual void computeGeometry(std::vector<Vertex>& pVtc, std::vector<unsigned int>& pIndc) const = 0;

	void setInitialRotations(float rollAngle, float pitchAngle, float yawAngle);
	void setInitialPosition(glm::vec3 position);
	void setInitialPosition(float x, float y, float z);
	void incrementRotations(float rollAngle, float pitchAngle, float yawAngle);
	void incrementPosition(glm::vec3 positionDelta);
	void incrementPosition(float deltaX, float deltaY, float deltaZ);
	void setScale(glm::vec3 scale);
	void setScale(float x, float y, float z);

protected:

	GLuint vtxVBO;  // the vertex buffer object which holds the vertices data
	GLuint vao;  // the vertex array object which holds all the data required for renderring.
	GLuint indVBO;	// index buffer for drawing the geometry
	float rollAngle;	// rotation around the z-axis
	float pitchAngle;	// rotation around the x-axis
	float yawAngle;		// rotation around the y-axis
	glm::vec3 position;
	glm::vec3 scale;
	int numIndices;

	static void computeNormals(const std::vector<unsigned int>& indices, std::vector<Vertex>& vertices);

};

