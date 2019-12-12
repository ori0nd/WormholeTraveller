//
// Wormhole Traveller
//
// world.h - defines World class that represents the game world
//

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma once
class World
{
public:
	World();
	~World();

	void setScale(glm::vec3 scale);
	void setRotations(float yaw, float pitch, float roll); // those are in degrees
	void setPosition(glm::vec3 position);

	void getWorldTransform(glm::mat4 * dest);

private:

	glm::vec3 scale;
	glm::vec3 position;
	float yaw, pitch, roll;

};

