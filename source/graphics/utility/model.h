#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "mesh.h"

// untextured vertex
class u_model {

public:

	std::vector<u_mesh> meshes;

	u_model();

	void init();

	void render(shader Shader);

	void cleanUp();
};

// textured vertex
class v_model {

public:

	std::vector<v_mesh> meshes;

	v_model();

	void init();

	void render(shader Shader);

	void cleanUp();
};

// .gltf
class m_model {

public:
	glm::vec3 position;
	glm::vec3 size;

	m_model(glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

	void init();
	void loadModel(std::string path);

	void render(shader Shader);

	void cleanUp();

protected:
	bool noTex;

	std::vector<m_mesh> meshes;
	std::string directory;

	std::vector<m_texture> texturesLoaded;

	void processNode(aiNode* node, const aiScene* scene);
	m_mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<m_texture> loadMaterialTextures(aiMaterial* material, aiTextureType type);
};


#endif
