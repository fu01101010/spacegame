#ifndef VMESH_H
#define VMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "shader.h"
#include "textures/vTexture.h"

struct vVertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	//glm::vec3 aColor;

	static std::vector<struct vVertex> genVList(float* vertices, int nVertices);
};

typedef struct vVertex vVertex;

class vMesh {

public:

	std::vector<vVertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<vTexture> textures;

	vMesh();
	vMesh(std::vector<vVertex> vertices, std::vector<unsigned int> indices, std::vector<vTexture> textures);

	void render(shader Shader);

	void cleanUp();
	
private:

	unsigned int VBO, EBO;
	
	void setUp();
};

#endif
