#include "stdafx.h"
#include "SphereObject.h"


SphereObject::SphereObject(int longs, int lats)
{
	longitudes = longs;
	latitudes = lats;
}


SphereObject::~SphereObject()
{
}

void SphereObject::computeGeometry(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const
{
	glm::vec4 pos;
	glm::vec4 col;
	float alpha;
	float beta;
	float deltaAlpha;
	float deltaBeta;

	int numRows = latitudes * 2;
	int numCols = longitudes;

	int numVtx = (numRows + 1) * (numCols + 1);
	vertices.resize(numVtx);

	int numTriangles = numRows * numCols * 2;
	indices.resize(numTriangles * 3);

	int i, j, k;

	k = 0;
	alpha = 0.0f;  // angle of latitude starting from the "south pole"
	deltaAlpha = (float)90.0 / latitudes; // increment of alpha
	beta = 0;   // angle of the longtidute 
	deltaBeta = (float)360.0 / (longitudes);	// increment of beta

	for (i = 0, alpha = -90; i <= numRows; i++, alpha += deltaAlpha) {
		for (j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
			pos.x = cos(glm::radians(alpha))*cos(glm::radians(beta));
			pos.y = cos(glm::radians(alpha))*sin(glm::radians(beta));
			pos.z = sin(glm::radians(alpha));
			pos.w = 1.0;

			vertices[k] = Vertex(pos, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			k++;
		}
	}

	k = 0;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			// fill indices for the quad
			// change by making a quad function
			indices[k++] = i * (numCols + 1) + j;
			indices[k++] = i * (numCols + 1) + j + 1;
			indices[k++] = (i + 1) * (numCols + 1) + j + 1;

			indices[k++] = i * (numCols + 1) + j;
			indices[k++] = (i + 1) * (numCols + 1) + j + 1;
			indices[k++] = (i + 1) * (numCols + 1) + j;
		}
	}

	computeNormals(indices, vertices);
}

void SphereObject::renderObject(ShaderProgram& shader) const
{
	glm::mat4 modelTransform;
	this->getModelTransform(&modelTransform);
	shader.copyMatrixToShader(glm::transpose(modelTransform), "model");
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}
