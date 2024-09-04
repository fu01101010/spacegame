#ifndef DMESH_H
#define DMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "shader.h"
#include "textures/dTexture.h"

struct vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	//glm::vec3 aColor;

	static std::vector<struct vertex> genList(float* vertices, int nVertices);
};

typedef struct vertex vertex;

class dMesh {

public:

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<dTexture> textures;

	DFLT_MESH();
	DFLT_MESH(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<dTexture> textures = {});

	void render(shader Shader);

	void cleanUp();

private:

	unsigned int VBO, EBO;

	void setUp();
};

#endif
