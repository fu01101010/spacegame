#ifndef WEIGHEDMESH_H
#define WEIGHEDMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>
#include <string>
#include <utility>

#include "../shader.h"
#include "../textures/texture.h"


#define MAX_BONES_PER_VERTEX 4



//
//
// weighed vertex struct here

struct weighedVertex {
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;

	glm::vec2 textureCoords;
	
	glm::vec4 wvBoneData;
	glm::vec4 weightData;
};

typedef struct weighedVertex weighedVertex;

//
//
// weighed mesh class here

class weighedMesh {

public:
	
	std::vector<weighedVertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO;

	std::vector<texture> textures;

	aiColor4D diff;
	aiColor4D spec;

	weighedMesh();
	weighedMesh(std::vector<weighedVertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures = {});
	weighedMesh(std::vector<weighedVertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec);

	void render(shader Shader);

	void cleanUp();

private:

	bool noTex;

	unsigned int VBO, EBO;

	void setUp();
};

#endif
