#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {

	Vertex() {}

	Vertex(const glm::vec4& pos)
	{
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		position[3] = pos.w;

		memset(color, 1.0f, sizeof(float) * 4);
		memset(normal, 0.0f, sizeof(float) * 4);
	}

	Vertex(const glm::vec4& pos, const glm::vec4& col)
	{
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		position[3] = pos.w;

		color[0] = col.x;
		color[1] = col.y;
		color[2] = col.z;
		color[3] = col.w;

		memset(normal, 0.0f, sizeof(float) * 4);
	}

	float position[4];
	float color[4];
	float normal[3];

};
