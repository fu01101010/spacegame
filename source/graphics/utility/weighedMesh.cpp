#include "weighedMesh.h"



//
//
// weighed vertex struct here
std::vector<weighedVertex> weighedVertex::genWVList(float* vertices, int nVertices, float* weights) {
	
	std::vector<weighedVertex> retval (nVertices);

	int stride = sizeof(weighedVertex) / sizeof(float);

	for (int i = 0; i < nVertices; ++i) {
		
		retval[i].position = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);

		retval[i].normal = glm::vec3(
			vertices[i * stride + 3],
			vertices[i * stride + 4],
			vertices[i * stride + 5]
		);
		
		retval[i].color = glm::vec3(
			vertices[i * stride + 6],
			vertices[i * stride + 7],
			vertices[i * stride + 8]
		);

		for (int j = 0; j < MAX_BONES_PER_VERTEX; ++j) {
			
			retval[i].weightData.at(j) = weights[j];	
		}
	}

	return retval;
}



//
//
// weighed mesh class here
weighedMesh::weighedMesh() {}


weighedMesh::weighedMesh(std::vector<weighedVertex> vertices, std::vector<unsigned int> indices) 
	: vertices(vertices), indices(indices) {

	setUp();
}


void weighedMesh::setUp() {
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(weighedVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set pointers here
	// .position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(weighedVertex), (void*)offsetof(weighedVertex, position));
	
	// .normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(weighedVertex), (void*)offsetof(weighedVertex, normal));

	// .color
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(weighedVertex), (void*)offsetof(weighedVertex, color));

	// .weightData
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, MAX_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(weighedVertex), (void*)offsetof(weighedVertex, weightData));

	glBindVertexArray(0);
}


void weighedMesh::cleanUp() {
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


void weighedMesh::render(shader Shader) {
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
