#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../mesh.h"

class u_model {

public:

	std::vector<mesh> meshes;

	u_model();

	void init();

	void render(shader Shader);

	void cleanUp();
};

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

	std::vector<mesh> meshes;
	std::string directory;

	std::vector<m_texture> texturesLoaded;

	void processNode(aiNode* node, const aiScene* scene);
	mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<m_texture> loadMaterialTextures(aiMaterial* material, aiTextureType type);
};

#endif
