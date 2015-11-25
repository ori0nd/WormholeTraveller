#include "stdafx.h"
#include "SceneObject.h"


SceneObject::SceneObject()
{
}


SceneObject::~SceneObject()
{
}

OpStatus SceneObject::createVao(ShaderProgram& sh, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	int rc = 0;

	GLint location;		// location of the attributes in the shader;

						//create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create vertex buffer object
	glGenBuffers(1, &vtxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vtxVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),	vertices.data(), GL_STATIC_DRAW);

	//copy the vertex position
	location = glGetAttribLocation(sh.getProgId(), "position");
	
	if (location == -1) {
		return OPS_UNIFORM_NOT_FOUND;
	}

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//glDisableVertexAttribArray(location);


	//copy the vertex color
	location = glGetAttribLocation(sh.getProgId(), "color");

	if (location == -1) {
		return OPS_UNIFORM_NOT_FOUND;
	}

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//glDisableVertexAttribArray(location);


	// copy the normal
	location = glGetAttribLocation(sh.getProgId(), "normal");

	if (location == -1) {
		return OPS_UNIFORM_NOT_FOUND;
	}

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//glDisableVertexAttribArray(location);


	//create index buffer
	glGenBuffers(1, &indVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	// store the number of indices
	numIndices = indices.size();

	//end creation
	glBindVertexArray(0);
	return OPS_OK;
}

void SceneObject::getModelTransform(glm::mat4 * dest) const
{
	glm::mat4 tRotate = glm::eulerAngleYXZ(yawAngle, pitchAngle, rollAngle);
	glm::mat4 tScale = glm::scale(glm::mat4(1.0f), this->scale);
	glm::mat4 tTranslate = glm::transpose(glm::translate(glm::mat4(1.0f), this->position));
	glm::mat4 model = tRotate * tScale * glm::mat4(1.0f);
	model[0].w = position.x;
	model[1].w = position.y;
	model[2].w = position.z;
	model[3].w = 1.0f;
 	*dest = model;
}

void SceneObject::renderObject(ShaderProgram& sh) const
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void SceneObject::setInitialRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle = rollAngle;
	this->pitchAngle = pitchAngle;
	this->yawAngle = yawAngle;
}

void SceneObject::setInitialPosition(glm::vec3 position)
{
	this->position = position;
}

void SceneObject::setInitialPosition(float x, float y, float z)
{
	this->position = glm::vec3(x, y, z);
}

void SceneObject::incrementRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle += rollAngle;
	this->pitchAngle += pitchAngle;
	this->yawAngle += yawAngle;
}

void SceneObject::incrementPosition(glm::vec3 delta)
{
	this->position += delta;
}

void SceneObject::incrementPosition(float deltaX, float deltaY, float deltaZ)
{
	this->position += glm::vec3(deltaX, deltaY, deltaZ);
}

void SceneObject::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

void SceneObject::setScale(float x, float y, float z)
{
	this->scale = glm::vec3(x, y, z);
}

void SceneObject::computeNormals(const std::vector<unsigned int>& indices, std::vector<Vertex>& vertices)
{
	for (unsigned int i = 0; i < indices.size(); i += 3) {
		unsigned int ivtxA = indices.at(i);
		unsigned int ivtxB = indices.at(i + 1);
		unsigned int ivtxC = indices.at(i + 2);

		Vertex& vtxA = vertices.at(ivtxA);
		Vertex& vtxB = vertices.at(ivtxB);
		Vertex& vtxC = vertices.at(ivtxC);

		glm::vec3 u = glm::vec3(vtxB.position[0], vtxB.position[1], vtxB.position[2]) -
			glm::vec3(vtxA.position[0], vtxA.position[1], vtxA.position[2]);

		glm::vec3 v = glm::vec3(vtxC.position[0], vtxC.position[1], vtxC.position[2]) -
			glm::vec3(vtxA.position[0], vtxA.position[1], vtxA.position[2]);

		glm::vec3 N = glm::normalize(glm::cross(u, v));

		vtxA.normal[0] += N.x;
		vtxA.normal[1] += N.y;
		vtxA.normal[2] += N.z;

		vtxB.normal[0] += N.x;
		vtxB.normal[1] += N.y;
		vtxB.normal[2] += N.z;

		vtxC.normal[0] += N.x;
		vtxC.normal[1] += N.y;
		vtxC.normal[2] += N.z;

		//std::cout << "setting normal: [" << N.x << ", " << N.y << ", " << N.z << "]" << std::endl;
	}

	// Now normalize each vertex' normal, since cross product doesn't guarantee normality
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		Vertex& v = vertices.at(i);
		glm::vec3 normal = glm::normalize(glm::vec3(v.normal[0], v.normal[1], v.normal[2]));

		v.normal[0] = normal.x;
		v.normal[1] = normal.y;
		v.normal[2] = normal.z;
	}
}
