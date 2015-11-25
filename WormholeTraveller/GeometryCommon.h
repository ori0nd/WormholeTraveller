#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {

	Vertex() {}

	Vertex(const glm::vec4& pos)
	{
		memcpy(position, glm::value_ptr(pos), sizeof(pos));
		memset(color, 1.0f, sizeof(float) * 4);
		memset(normal, 0.0f, sizeof(float) * 4);
	}

	Vertex(const glm::vec4& pos, const glm::vec4& col)
	{
		memcpy(position, glm::value_ptr(pos), sizeof(pos));
		memcpy(position, glm::value_ptr(col), sizeof(col));
		memset(normal, 0.0f, sizeof(float) * 4);
	}

	float position[4];
	float color[4];
	float normal[3];

};
