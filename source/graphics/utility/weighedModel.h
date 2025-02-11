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
// weighed model class here
class weighedModel {
	
public:
	
	glm::vec3 position;
	glm::vec3 size;

	int nAnims;
	int currentAnim;

	weighedModel(glm::vec3 position = glm::vec3(0.0f), glm::vec3 size = glm::vec3(0.0f), bool noTex = false);

	std::vector<std::string> boneMap;

	void init();

	void load(std::string filepath);

	void render(shader Shader, float time);

	void cleanUp();

	void populateBoneTransforms(std::vector<aiMatrix4x4>& boneTransforms, float time);
protected:
	
	Assimp::Importer importer;
	const aiScene* scene = nullptr;

	bool noTex;

	std::vector<weighedMesh> meshes;
	std::string directory;

	std::vector<texture> texturesLoaded;
	std::vector<texture> loadMaterialTextures(aiMaterial* material, aiTextureType type);

	void processScene(const aiScene* scene);
	weighedMesh processMesh(aiMesh* mesh);

	std::vector<std::vector<std::pair<int, float> >> vertexMap;	

	void processBone(unsigned int boneID, const aiBone* bone);

	struct boneDataStruct {
		
		// uses assimp aiMatrix4x4 data type. will prob create a standalone struct to manage it in the future. 
		aiMatrix4x4 offsetMatrix;
		aiMatrix4x4 endTransform;

		// constructor
		boneDataStruct(const aiMatrix4x4& offset) { offsetMatrix = offset; setZeros(endTransform);}

		void setZeros(aiMatrix4x4& mat) {
			
			mat = aiMatrix4x4(0.0f, 0.0f, 0.0f, 0.0f, 
					  0.0f, 0.0f, 0.0f, 0.0f, 
					  0.0f, 0.0f, 0.0f, 0.0f, 
					  0.0f, 0.0f, 0.0f, 0.0f);
		}
	};

	std::vector<boneDataStruct> boneData;
	
	void readNodeHierarchy(const aiNode* node, const aiMatrix4x4& parentTransform, float time);

	aiMatrix4x4 globalInverseTransform;

	const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string name);

	unsigned int findTranslationIndex(float time, const aiNodeAnim* nodeAnim);
	void calculateInterpolatedTranslation(aiVector3D& translation, float time, const aiNodeAnim* nodeAnim);

	unsigned int findRotationIndex(float time, const aiNodeAnim* nodeAnim);
	void calculateInterpolatedRotation(aiQuaternion& rotation, float time, const aiNodeAnim* nodeAnim);

	unsigned int findScalingIndex(float time, const aiNodeAnim* nodeAnim);
	void calculateInterpolatedScaling(aiVector3D& scaling, float time, const aiNodeAnim* nodeAnim);
};

#endif
