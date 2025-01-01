#include "mesh.h"



//
//
// untextured u_vertex
std::vector<u_vertex> u_vertex::genUList(float* vertices, int nVertices) {

	std::vector<u_vertex> retval(nVertices);

	int stride = sizeof(u_vertex) / sizeof(float);

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
		
		retval[i].aColor = glm::vec3(
			vertices[i * stride + 6],
			vertices[i * stride + 7],
			vertices[i * stride + 8]
		);
	}

	return retval;
}

u_mesh::u_mesh() {}

u_mesh::u_mesh(std::vector<u_vertex> vertices, std::vector<unsigned int> indices) 
	: vertices(vertices), indices(indices) {

	setUp();
}

void u_mesh::render(shader Shader) {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void u_mesh::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void u_mesh::setUp() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(u_vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//SET POINTERS
	//u_vertex.Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(u_vertex), (void*)offsetof(u_vertex, position));

	//u_vertex.Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(u_vertex), (void*)offsetof(u_vertex, normal));

	//u_vertex.aColor
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(u_vertex), (void*)offsetof(u_vertex, aColor));

	glBindVertexArray(0);
}



//
//
// textured u_vertex & .gltf
std::vector<vertex> vertex::genVList(float* vertices, int nVertices) {

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
	}

	return retval;
}

v_mesh::v_mesh() {}

v_mesh::v_mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<v_texture> textures) 
	: vertices(vertices), indices(indices), textures(textures) {

	setUp();
}

void v_mesh::render(shader Shader) {

	//textures
	for (unsigned int i = 0; i < textures.size(); ++i) {

		Shader.set_int(textures[i].texName, textures[i].texID);

		textures[i].activate();

		glBindTexture(GL_TEXTURE_2D, textures[i].texID);
		// make it into a function (tex[i].bind())
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void v_mesh::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void v_mesh::setUp() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into u_vertex buffers


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.

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

	glBindVertexArray(0);
}

//
// .gltf
m_mesh::m_mesh() {}

m_mesh::m_mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<m_texture> textures)
	: vertices(vertices), indices(indices), textures(textures), noTex(false) {

	setUp();
}

m_mesh::m_mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec)
	: vertices(vertices), indices(indices), diff(diff), spec(spec), noTex(true) {
	
	setUp();
}

void m_mesh::render(shader Shader) {

	if (noTex) {

		Shader.set4flt("Material.diffuse", diff);
		Shader.set4flt("Material.specular", spec);
		Shader.set_int("noTex", 1);
	}
	else {
		unsigned int diffuseIDx = 0;
		unsigned int specularIDx = 0;

		for (unsigned int i = 0; i < textures.size(); ++i) {

			//activate textures
			glActiveTexture(GL_TEXTURE0 + i);

			//retreive texture info
			std::string number;
			std::string name;

			switch (textures[i].type) {

				case aiTextureType_DIFFUSE: {
					name = "diffuse";
					number = std::to_string(diffuseIDx++);
					break;
				}
				case aiTextureType_SPECULAR: {
					name = "specular";
					number = std::to_string(specularIDx++);
					break;
				}
				default: {

				}
			}

			//set texture values in the shader
			Shader.set_int(name + number, i);

			//bind textures
			textures[i].bind();
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void m_mesh::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void m_mesh::setUp() {

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
