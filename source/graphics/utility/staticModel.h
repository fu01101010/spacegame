#ifndef STATICMODEL_H
#define STATICMODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "staticMesh.h"



//
//
// static model class here
class staticModel {

public:
	
	glm::vec3 position;
	glm::vec3 size;

	staticModel(glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

	void init();
	void load(std::string path);

	void render(shader Shader);

	void cleanUp();

protected:

	Assimp::Importer importer;
	const aiScene* scene = nullptr;
	
	bool noTex;

	std::vector<staticMesh> meshes;
	std::string directory;

	std::vector<texture> texturesLoaded;

	void processScene(const aiScene* scene);
	staticMesh processMesh(aiMesh* mesh);

	std::vector<texture> loadMaterialTextures(aiMaterial* material, aiTextureType type);
};

#endif
