#include "mesh.h"

std::vector<vertex> vertex::genList(char type, float* vertices, int nVertices) {

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
		
		// vertex no_texture
		if (type == 'u') {
			
			retval[i].aColor = glm::vec3(
			vertices[i * stride + 6],
			vertices[i * stride + 7],
			vertices[i * stride + 8]
			);
		// textured vertex or basic model textures
		} else if (type == 'v' || type == 'm') {

			retval[i].texCoord = glm::vec2(
			vertices[i * stride + 9],
			vertices[i * stride + 10]
			);
		}
	}

	return retval;
}

mesh::mesh() {}

mesh::mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices) 
	: vertices(vertices), indices(indices), noTex(true) {

	setUp();
}

mesh::mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<v_texture> vTextures) 
	: vertices(vertices), indices(indices), vTextures(vTextures), noTex(false) {

	setUp();
}

mesh::mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<m_texture> mTextures)
	: vertices(vertices), indices(indices), mTextures(mTextures), noTex(false) {

	setUp();
}

mesh::mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec)
	: vertices(vertices), indices(indices), diff(diff), spec(spec), noTex(true) {
	
	setUp();
}


void mesh::render(shader Shader) {
	
	if (type == 'v') {

		// textured vertex
		for (unsigned int i = 0; i < vTextures.size(); ++i) {

			Shader.set_int(vTextures[i].texName, vTextures[i].texID);

			vTextures[i].activate();

			glBindTexture(GL_TEXTURE_2D, vTextures[i].texID);
			// make it into a function (tex[i].bind())
		}
	} else if (type == 'm') {
		
		if (noTex) {

			Shader.set4flt("Material.diffuse", diff);
			Shader.set4flt("Material.specular", spec);
			Shader.set_int("noTex", 1);
		}
		else {
			unsigned int diffuseIDx = 0;
			unsigned int specularIDx = 0;

			for (unsigned int i = 0; i < mTextures.size(); ++i) {

				//activate textures
				glActiveTexture(GL_TEXTURE0 + i);

				//retreive texture info
				std::string number;
				std::string name;

				switch (mTextures[i].type) {

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
				mTextures[i].bind();
			}
		}
	}
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void mesh::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void mesh::setUp() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//SET POINTERS
	//vertex.Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));

	//vertex.Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
	
	if (type == 'u') {
		
		//vertex.aColor
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, aColor));
	} else if (type == 'v' || type == 'm') {
		
		//vVertex.texCoord
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texCoord));
	}
	
	glBindVertexArray(0);
}

