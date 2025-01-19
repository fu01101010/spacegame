#ifndef WEIGHEDMESH_H
#define WEIGHEDMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>
#include <utility>

#include "../shader.h"

#define MAX_BONES_PER_VERTEX 4



//
//
// weighed vertex struct here
struct weighedVertex {
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	
	glm::vec4 wvBoneData;
	glm::vec4 weightData;
};

typedef struct weighedVertex weighedVertex;

class weighedMesh {

public:
	
	std::vector<weighedVertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO;

	weighedMesh();
	weighedMesh(std::vector<weighedVertex> vertices, std::vector<unsigned int> indices);

	void render(shader Shader);

	void cleanUp();

private:

	unsigned int VBO, EBO;

	void setUp();
};

#endif
