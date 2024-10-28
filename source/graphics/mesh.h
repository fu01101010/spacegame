#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "shader.h"

#include "texture.h"

struct vertex {

	glm::vec3 position;
	glm::vec3 normal;	
	glm::vec3 aColor;
	glm::vec2 texCoord;

	static std::vector<struct vertex> genList(char type, float* vertices, int nVertices);
};

typedef struct vertex vertex;

class mesh {

public:
	char type;

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<v_texture> vTextures;
	std::vector<m_texture> mTextures;

	aiColor4D diff;
	aiColor4D spec;

	mesh();
	mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices); 
	mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<v_texture> vTextures);
	mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<m_texture> mTextures);
	mesh(char type, std::vector<vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec);

	void render(shader Shader);

	void cleanUp();
	
private:
	bool noTex;

	unsigned int VBO, EBO;
	
	void setUp();
};

#endif

