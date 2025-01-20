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

	void getBoneTransforms(std::vector<aiMatrix4x4>& boneTransforms, float time);
protected:

	Assimp::Importer importer;
	const aiScene* scene = nullptr;

	std::vector<weighedMesh> meshes;
	std::string directory;

	void processScene(const aiScene* scene);
	weighedMesh processMesh(aiMesh* mesh);

	std::vector<std::vector<std::pair<int, float> >> vertexMap;

	void processBone(unsigned int boneID, const aiBone* bone);

	void readNodeHierarchy(const aiNode* node, const aiMatrix4x4& parentTransform, float time);

	struct boneDataStruct {
		
		aiMatrix4x4 offsetMatrix;
		aiMatrix4x4 endTransform;

		// constructor
		boneDataStruct(const aiMatrix4x4& offset) { offsetMatrix = offset; setZeros(endTransform);}

		void setZeros(aiMatrix4x4& mat) {
			
			mat = aiMatrix4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		}
	};
	
	aiMatrix4x4 GLMMatrixToAssimp(glm::mat4 mat);
	aiMatrix4x4 globalInverseTransform;

	std::vector<boneDataStruct> boneData;

	const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string name);

	unsigned int findTranslationIndex(float time, const aiNodeAnim* nodeAnim);
	void calculateInterpolatedTranslation(aiVector3D& translation, float time, const aiNodeAnim* nodeAnim);

	unsigned int findRotationIndex(float time, const aiNodeAnim* nodeAnim);
	void calculateInterpolatedRotation(aiQuaternion& rotation, float time, const aiNodeAnim* nodeAnim);

	unsigned int findScalingIndex(float time, const aiNodeAnim* nodeAnim);
	void calculateInterpolatedScaling(aiVector3D& scaling, float time, const aiNodeAnim* nodeAnim);

	};
#endif
