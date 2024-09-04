#include "dMesh.h"

std::vector<vertex> vertex::genList(float* vertices, int nVertices) {

	std::vector<vertex> retval(nVertices);

	int stride = sizeof(vertex) / sizeof(float);

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

dMesh::dMesh() {}

dMesh::dMesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<dTexture> textures)
	: vertices(vertices), indices(indices), textures(textures) {

	setUp();
}

void dMesh::render(shader Shader) {

	unsigned int diffuseIDx = 0;
	unsigned int specularIDx = 0;

	for (unsigned int i = 0; i < textures.size(); ++i) {

		//activate textures
		glActiveTexture(GL_TEXTURE0 + i);

		//retreive texture info
		std::string name;

		switch (textures[i].type) {

		case aiTextureType_DIFFUSE: {
			name = "diffuse" + std::to_string(DiffuseIDx++);
			break;
		}
		case aiTextureType_SPECULAR: {
			name = "specular" + std::to_string(SpecularIDx++);
			break;
		}
		}

		//set texture values in the shader
		Shader.set_int(name, i);

		//bind textures
		textures[i].bind();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void dMesh::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void dMesh::setUp() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//set pointers	
	//vertex.position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));

	//vertex.normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

	//vertex.texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texCoord));

	//vertex.aColor
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, aColor));

	glBindVertexArray(0);
}
