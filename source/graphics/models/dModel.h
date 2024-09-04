#ifndef DFLT_MODEL_H
#define DFLT_MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../../Meshes/DFLT_Mesh.h"

class DFLT_MODEL {

public:
	glm::vec3 Position;
	glm::vec3 Size;

	DFLT_MODEL();
	DFLT_MODEL(glm::vec3 Position = glm::vec3(0.0f), glm::vec3 Size = glm::vec3(1.0f));

	void INIT();
	void LOADModel(std::string PATH);

	void RENDER(SHADER SHADER);

	void CleanUp();

protected:
	std::vector<DFLT_MESH> MESHES;
	std::string DIRECTORY;

	std::vector<DFLT_TEXTURE> TexturesLoaded;

	void ProcessNODE(aiNode* node, const aiScene* Scene);
	DFLT_MESH ProcessMESH(aiMesh* mesh, const aiScene* Scene);

	std::vector<DFLT_TEXTURE> LoadTextures(aiMaterial* material, aiTextureType type);
};

#endif