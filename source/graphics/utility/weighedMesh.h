#ifndef WEIGHEDMESH_H
#define WEIGHEDMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>
#include <utility>

#include "../shader.h"


#define MAX_BONES_PER_VERTEX 26



//
//
// weighed vertex struct here
struct weighedVertex {
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;

	std::vector<float> weightData = std::vector<float>(MAX_BONES_PER_VERTEX);

	static std::vector<struct weighedVertex> genWVList(float* vertices, int nVertices, float* weights);
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
