#ifndef VMESH_NOTEX_H
#define VMESH_NOTEX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "shader.h"

struct vertex {

	glm::vec3 position;
	glm::vec3 normal;	
	glm::vec3 aColor;

	static std::vector<struct vertex> genList(float* vertices, int nVertices);
};

typedef struct vertex vertex;

class vMesh_notex {

public:

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	vMesh_notex();
	vMesh_notex(std::vector<vertex> vertices, std::vector<unsigned int> indices);

	void render(shader Shader);

	void cleanUp();
	
private:

	unsigned int VBO, EBO;
	
	void setUp();
};

#endif

