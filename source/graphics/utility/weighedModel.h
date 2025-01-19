#ifndef WEIGHEDMODEL_H
#define WEIGHEDMODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <utility>

#include "weighedMesh.h"



//
//
// weighed model class
class weighedModel {
	
public:
	
	glm::vec3 position;
	glm::vec3 size;

	weighedModel(glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(0.0f));

	void init();

	void load(std::string filepath);

	void render(shader Shader);

	void cleanUp();

	std::vector<std::string> boneMap;
protected:

	std::vector<weighedMesh> meshes;
	std::string directory;

	void processScene(const aiScene* scene);
	weighedMesh processMesh(aiMesh* mesh);

	std::vector<std::vector<std::pair<int, float> >> vertexMap;

	void processBone(unsigned int boneID, const aiBone* bone);
};
#endif
