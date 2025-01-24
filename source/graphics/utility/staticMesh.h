#ifndef STATICMESH_H
#define STATICMESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>
#include <string>
#include <utility>

#include "../shader.h"
#include "../textures/texture.h"



//
//
// static vertex struct here

struct staticVertex {
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;

	glm::vec2 textureCoords;	
};

typedef struct staticVertex staticVertex;

//
//
// static mesh class here

class staticMesh {

public:
	
	std::vector<staticVertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO;

	std::vector<texture> textures;

	aiColor4D diff;
	aiColor4D spec;

	staticMesh();
	staticMesh(std::vector<staticVertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures = {});
	staticMesh(std::vector<staticVertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec);

	void render(shader Shader);

	void cleanUp();

private:

	bool noTex;

	unsigned int VBO, EBO;

	void setUp();
};

#endif
