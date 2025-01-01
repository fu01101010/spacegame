#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>

#include "../shader.h"
#include "../textures/texture.h"



//
//
// untextured mesh class here
struct u_vertex {

	glm::vec3 position;
	glm::vec3 normal;	
	glm::vec3 aColor;

	static std::vector<struct u_vertex> genUList(float* vertices, int nVertices);
};

typedef struct u_vertex u_vertex;

class u_mesh {

public:

	std::vector<u_vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	u_mesh();
	u_mesh(std::vector<u_vertex> vertices, std::vector<unsigned int> indices);

	void render(shader Shader);

	void cleanUp();
	
private:

	unsigned int VBO, EBO;
	
	void setUp();
};



//
//
// textured vertex and .gltf classes here
struct vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	static std::vector<struct vertex> genVList(float* vertices, int nVertices);
};

typedef struct vertex vertex;

//
// vertex
class v_mesh {

public:

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<v_texture> textures;

	v_mesh();
	v_mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<v_texture> textures = {});

	void render(shader Shader);

	void cleanUp();
	
private:

	unsigned int VBO, EBO;
	
	void setUp();
};

//
// .gltf
class m_mesh {

public:

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<m_texture> textures;

	aiColor4D diff;
	aiColor4D spec;
	
	m_mesh();
	m_mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<m_texture> textures = {});
	m_mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec);

	void render(shader Shader);

	void cleanUp();

private:

	bool noTex;

	unsigned int VBO, EBO;

	void setUp();
};


#endif
