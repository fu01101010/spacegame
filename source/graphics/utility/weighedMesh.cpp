#include "weighedMesh.h"



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
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(weighedVertex), (void*)offsetof(weighedVertex, color));
	
	// somehow cannot push GL_INT via ivec4 to shader, maybe will fix later. works for now with GL_FLOAT via vec4.

	// .wvboneData works together with .weightData
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, MAX_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(weighedVertex), (void*)offsetof(weighedVertex, wvBoneData));

	// .weightData
	glEnableVertexAttribArray(4);
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
