

#include "stdafx.h"
#include "SceneObject.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

//OpStatus SceneObject::createVao(ShaderProgram& sh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
//{
//	GLint location;		// location of the attributes in the shader;
//
//						//create vertex array object
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	//create vertex buffer object
//	glGenBuffers(1, &vtxVBO);
//	glBindBuffer(GL_ARRAY_BUFFER, vtxVBO);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),	vertices.data(), GL_STATIC_DRAW);
//
//	//copy the vertex position
//	location = glGetAttribLocation(sh.getProgId(), "position");
//	
//	if (location == -1) {
//		return OPS_UNIFORM_NOT_FOUND;
//	}
//
//	glEnableVertexAttribArray(location);
//	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
//	//glDisableVertexAttribArray(location);
//
//	// copy the normal
//	location = glGetAttribLocation(sh.getProgId(), "normal");
//
//	if (location == -1) {
//		return OPS_UNIFORM_NOT_FOUND;
//	}
//
//	glEnableVertexAttribArray(location);
//	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//	//glDisableVertexAttribArray(location);
//
//
//	//create index buffer
//	glGenBuffers(1, &indVBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indVBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
//	// store the number of indices
//	numIndices = indices.size();
//
//	//end creation
//	glBindVertexArray(0);
//	return OPS_OK;
//}

OpStatus SceneObject::createVao(ShaderProgram & sh, vector<vec3> * vertices, vector<vec2> * uvs, vector<vec3> * normals)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	numVertices = vertices->size();

	// delete below
	GLint location;		// location of the attributes in the shader;

						//create vertex array object


	//create vertex buffer object
	glGenBuffers(1, &vtxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vtxVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(vec3), &(vertices->at(0)), GL_STATIC_DRAW);

	//copy the vertex position
	location = glGetAttribLocation(sh.getProgId(), "position");

	if (location == -1) {
		cerr << "cannot find uniform: position" << endl;
		return OPS_UNIFORM_NOT_FOUND;
	}

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (normals != NULL && normals->size() > 0)
	{
		// create normals VBO
		glGenBuffers(1, &normalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(vec3), &(normals->at(0)), GL_STATIC_DRAW);

		// copy the normal
		location = glGetAttribLocation(sh.getProgId(), "normal");

		if (location == -1) {
			cerr << "cannot find uniform: normal" << endl;
			return OPS_UNIFORM_NOT_FOUND;
		}

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	if (uvs != NULL && uvs->size() > 0)
	{
		// create uvs VBO
		glGenBuffers(1, &uvsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
		glBufferData(GL_ARRAY_BUFFER, uvs->size() * sizeof(vec2), &(uvs->at(0)), GL_STATIC_DRAW);

		// copy the UV
		location = glGetAttribLocation(sh.getProgId(), "uv");

		if (location == -1) {
			cerr << "cannot find uniform: uv" << endl;
			return OPS_UNIFORM_NOT_FOUND;
		}

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	//end creation
	glBindVertexArray(0);
	return OPS_OK;
}

OpStatus SceneObject::createVao(ShaderProgram & sh, vector<vec3> * vertices)
{
	return createVao(sh, vertices, NULL, NULL);
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

void SceneObject::renderObject() const
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
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

vec3 SceneObject::getPosition()
{
	return position;
}

OpStatus SceneObject::loadOBJ(const char * path, std::vector<vec3>& out_vertices, std::vector<vec2>& out_uvs, std::vector<vec3>& out_normals)
{
	char thePath[100];
	strcpy_s(thePath, path);
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3> tmp_vertices;
	std::vector<vec2> tmp_uvs;
	std::vector<vec3> tmp_normals;

	FILE * file;
	fopen_s(&file, thePath, "r");

	if (file == NULL)
	{
		cerr << "Cannot open OBJ: " << thePath << endl;
		return OPS_NOK;
	}

	while (true) {
		char lineHeader[128];

		// read first word
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // else parse header

		if (strcmp(lineHeader, "v") == 0)
		{
			vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			tmp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9) {
				cerr << "Error parsing OBJ file " << path << endl << "Use different parser instead, good luck :D" << endl;
				return OPS_NOK;
			}

			vertexIndices.push_back	(vertexIndex[0]);
			vertexIndices.push_back	(vertexIndex[1]);
			vertexIndices.push_back	(vertexIndex[2]);
			uvIndices.push_back		(uvIndex[0]);
			uvIndices.push_back		(uvIndex[1]);
			uvIndices.push_back		(uvIndex[2]);
			normalIndices.push_back	(normalIndex[0]);
			normalIndices.push_back	(normalIndex[1]);
			normalIndices.push_back	(normalIndex[2]);
		} 
		else if (strcmp(lineHeader, "o") == 0) {
			char name[25];
			fscanf(file, "%s\n", &name);
			cout << "reading OBJ object: " << name << "..." << endl;
		}
	}

	// For each vtx of each trig
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = tmp_vertices[vertexIndex - 1]; // OBJ starts indexing with 1
		out_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		unsigned int uvIndex = uvIndices[i];
		vec2 uv = tmp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		vec3 normal = tmp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

	return OPS_OK;
}

//void SceneObject::computeNormals(const std::vector<unsigned int>& indices, std::vector<Vertex>& vertices)
//{
//	for (unsigned int i = 0; i < indices.size(); i += 3) {
//		unsigned int ivtxA = indices.at(i);
//		unsigned int ivtxB = indices.at(i + 1);
//		unsigned int ivtxC = indices.at(i + 2);
//
//		Vertex& vtxA = vertices.at(ivtxA);
//		Vertex& vtxB = vertices.at(ivtxB);
//		Vertex& vtxC = vertices.at(ivtxC);
//
//		glm::vec3 u = glm::vec3(vtxB.position[0], vtxB.position[1], vtxB.position[2]) -
//			glm::vec3(vtxA.position[0], vtxA.position[1], vtxA.position[2]);
//
//		glm::vec3 v = glm::vec3(vtxC.position[0], vtxC.position[1], vtxC.position[2]) -
//			glm::vec3(vtxA.position[0], vtxA.position[1], vtxA.position[2]);
//
//		glm::vec3 N = glm::normalize(glm::cross(u, v));
//
//		vtxA.normal[0] += N.x;
//		vtxA.normal[1] += N.y;
//		vtxA.normal[2] += N.z;
//
//		vtxB.normal[0] += N.x;
//		vtxB.normal[1] += N.y;
//		vtxB.normal[2] += N.z;
//
//		vtxC.normal[0] += N.x;
//		vtxC.normal[1] += N.y;
//		vtxC.normal[2] += N.z;
//
//		//std::cout << "setting normal: [" << N.x << ", " << N.y << ", " << N.z << "]" << std::endl;
//	}
//
//	// Now normalize each vertex' normal, since cross product doesn't guarantee normality
//	for (unsigned int i = 0; i < vertices.size(); ++i) {
//		Vertex& v = vertices.at(i);
//		glm::vec3 normal = glm::normalize(glm::vec3(v.normal[0], v.normal[1], v.normal[2]));
//
//		v.normal[0] = normal.x;
//		v.normal[1] = normal.y;
//		v.normal[2] = normal.z;
//	}
//}
