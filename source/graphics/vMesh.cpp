#include "vMesh.h"

std::vector<vVertex> vVertex::genVList(float* vertices, int nVertices) {

	std::vector<vVertex> retval(nVertices);

	int stride = sizeof(vVertex) / sizeof(float);

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

		retval[i].texCoord = glm::vec2(
			vertices[i * stride + 6],
			vertices[i * stride + 7]
		);
		/*
		retval[i].aColor = glm::vec3(
			vertices[i * stride + 8],
			vertices[i * stride + 9],
			vertices[i * stride + 10]
		);
		*/
	}

	return retval;
}

vMesh::vMesh() {}

vMesh::vMesh(std::vector<vVertex> vertices, std::vector<unsigned int> indices, std::vector<vTexture> textures) 
	: vertices(vertices), indices(indices), textures(textures) {

	setUp();
}

void vMesh::render(shader Shader) {

	//textures
	for (unsigned int i = 0; i < textures.size(); ++i) {

		Shader.set_int(textures[i].tex_name, textures[i].tex_id);

		glActiveTexture(GL_TEXTURE0 + i);
		textures[i].bind();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void vMesh::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void vMesh::setUp() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//set pointers
	//vVertex.position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vVertex), (void*)offsetof(vVertex, position));

	//vVertex.normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vVertex), (void*)offsetof(vVertex, normal));

	//vVertex.texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vVertex), (void*)offsetof(vVertex, texCoord));

	//vVertex.aColor
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vVertex), (void*)offsetof(vVertex, aColor));

	glBindVertexArray(0);
}
