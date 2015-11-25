#pragma once

#include <iostream>

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	void setCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector);
	void setWindowDims(int width, int height);
	void changeAbsPosition(float x, float y, float z);
	void changePositionDelta(float dx, float dy, float dz);

	glm::vec3 moveForwardRelative(float numUnits);
	glm::vec3 moveRightRelative(float numUnits);

	void setOrientation(glm::vec3 lookAtVector, glm::vec3 upVector);
	void setOrientation(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector);

	void roll(float degrees);
	void pitch(float degrees);
	void yaw(float degrees);

	void zoomIn(float zoomAmount);

	void getViewMatrix(glm::mat4 * dest);
	void getProjectionMatrix(glm::mat4 * dest);

	void getViewerPosition(glm::vec4 * dest);

private:
	glm::vec4 position;
	glm::vec4 upVector;
	glm::vec4 lookAtVector;

	int winHeight, winWidth;

	float fov;
};

