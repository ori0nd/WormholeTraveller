#ifdef _WIN32
#include "stdafx.h"
#endif

#include "Camera.h"

Camera::Camera() : winWidth(1), winHeight(1), fov(CAM_DEFAULT_FOV)
{
}


Camera::~Camera()
{
}

void Camera::setCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector)
{
	this->position = glm::vec4(position, 1.0f);
	this->upVector = glm::vec4(upVector, 1.0f);
	glm::vec3 lookAtVector = lookAtPoint - position;
	this->lookAtVector = glm::vec4(lookAtVector, 1.0f);
	this->lookAtVector = glm::normalize(this->lookAtVector);
	this->upVector = glm::normalize(this->upVector);
}

void Camera::setWindowDims(int width, int height)
{
	this->winWidth = width;
	this->winHeight = height;
}

void Camera::changeAbsPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Camera::changePositionDelta(float dx, float dy, float dz)
{
	this->position.x += dx;
	this->position.y += dy;
	this->position.z += dz;
}

glm::vec3 Camera::moveForwardRelative(float numUnits)
{
	this->position += this->lookAtVector * numUnits;
	return glm::vec3(position.x, position.y, position.z);
}

glm::vec3 Camera::moveRightRelative(float numUnits)
{
	// TODO: Investigate wether it's ok to truncate the `w`, or we need to divide xyz by w instead??
	glm::vec3 rightVec = glm::normalize(glm::cross(glm::vec3(this->lookAtVector), glm::vec3(this->upVector)));
	this->position += glm::vec4(rightVec * numUnits, 1.0f);
	return this->position.xyz();
}

void Camera::setOrientation(glm::vec3 lookAtVector, glm::vec3 upVector)
{
	glm::vec3 lookAtPoint = lookAtVector.xyz() + this->position.xyz();
	this->setCamera(this->position.xyz(), lookAtPoint, upVector);
}

void Camera::setOrientation(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector)
{
	this->setCamera(position, lookAtPoint, upVector);
}

void Camera::roll(float degrees)
{
	glm::vec4 rotationVec = -this->lookAtVector;
	glm::mat4 rotatiomMat = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), glm::vec3(rotationVec));
	glm::vec4 u = rotatiomMat * upVector;
	this->upVector = u;
}

void Camera::pitch(float degrees)
{
	glm::vec3 lookAt3d = glm::vec3(this->lookAtVector);
	glm::vec3 upVec3d = glm::vec3(this->upVector);
	glm::vec3 rotationVec = glm::cross(lookAt3d, upVec3d); // aka x-axis (camera sp)
	glm::mat4 rotatiomMat = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), rotationVec);
	this->lookAtVector = glm::normalize(rotatiomMat * this->lookAtVector);
}

void Camera::yaw(float degrees)
{
	glm::vec3 rotationVec = glm::vec3(this->upVector);
	glm::mat4 rotatiomMat = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), rotationVec);
	this->lookAtVector = rotatiomMat * this->lookAtVector;
}

void Camera::zoomIn(float zoomAmount)
{
	float newFov = this->fov + zoomAmount;
	if (newFov >= 10.0f && newFov <= 100.0f)
	{
		this->fov = newFov;
	}
}

void Camera::resetZoom()
{
	this->fov = CAM_DEFAULT_FOV;
}

void Camera::getViewMatrix(glm::mat4 * viewMatrix)
{
	glm::vec3 lookAtPoint = glm::vec3(lookAtVector + position);
	
	//glm::mat4 cameraMatrix;

	//glm::vec4 u, v, n;

	//n = position - glm::vec4(lookAtPoint, 0.0f);
	//n.w = 0;
	//n = glm::normalize(n);

	//upVector.w = 0;
	//upVector = glm::normalize(upVector);

	//u = glm::normalize(
	//		glm::vec4(
	//			glm::cross(
	//					glm::vec3(upVector.x, upVector.y, upVector.z), 
	//					glm::vec3(n.x, n.y, n.z)
	//				), 
	//			0.0f
	//		)
	//);

	//v = glm::vec4(
	//	glm::cross(
	//		glm::vec3(n.x, n.y, n.z),
	//		glm::vec3(u.x, u.y, u.z)
	//	),
	//	0.0f
	//);

	//cameraMatrix[0] = u;
	//cameraMatrix[1] = v;
	//cameraMatrix[2] = n;
	//cameraMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//cameraMatrix[0].w = glm::dot(-u, position);
	//cameraMatrix[1].w = glm::dot(-v, position);
	//cameraMatrix[2].w = glm::dot(-n, position);

	glm::mat4 cameraMatrix = glm::lookAt(glm::vec3(position), glm::vec3(lookAtPoint), glm::vec3(upVector));
	*viewMatrix = glm::transpose(cameraMatrix);
}

void Camera::getProjectionMatrix(glm::mat4 * projMatrix)
{
	float aspectRatio = ((float)winWidth) / ((float)winHeight);
	glm::mat4 projection = glm::perspective(glm::radians(this->fov), aspectRatio, 0.1f, 15000.0f);
	*projMatrix = projection;
}

void Camera::getViewerPosition(glm::vec4* dest)
{
	*dest = this->position;
}
