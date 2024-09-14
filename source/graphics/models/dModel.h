#ifndef DMODEL_H
#define DMODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../dMesh.h"

class dModel {

public:
	glm::vec3 position;
	glm::vec3 size;

	dModel();
	dModel(glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));

	void init();
	void loadModel(std::string path);

	void render(shader Shader);

	void cleanUp();

protected:
	std::vector<dMesh> meshes;
	std::string directory;

	std::vector<dTexture> texturesLoaded;

	void processNode(aiNode* node, const aiScene* scene);
	dMesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<dTexture> loadTextures(aiMaterial* material, aiTextureType type);
};

#endif
