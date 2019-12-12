#ifdef _WIN32
#include "stdafx.h"
#endif

#include "World.h"

using namespace glm;

World::World()
{
	setScale(vec3(1.0f));
	setPosition(vec3(0.0f));
	yaw = pitch = roll = 0.0f;
}

World::~World()
{
}

void World::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

void World::setRotations(float yaw, float pitch, float roll)
{
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
}

void World::setPosition(glm::vec3 position)
{
	this->position = position;
}

void World::getWorldTransform(glm::mat4 * dest)
{
	mat4 tRotate = eulerAngleYXZ(yaw, pitch, roll);
	mat4 tScale = glm::scale(mat4(1.0f), this->scale);
	mat4 tTranslate = glm::translate(mat4(1.0f), this->position);

	*dest = tTranslate * tRotate * tScale * mat4(1.0f);
}
