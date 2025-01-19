#include "weighedModel.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs)

weighedModel::weighedModel(glm::vec3 position, glm::vec3 size)
	: position(position), size(size) { }

void weighedModel::init() {}

void weighedModel::render(shader Shader) {
	
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::scale(model, size);

	Shader.setmat4("model", model);
	//Shader.set_flt("Material.reflectivity", 0.5f);
	
	for (weighedMesh mesh : meshes) {
		
		mesh.render(Shader);
	}
}

void weighedModel::cleanUp() {
	
	for (weighedMesh mesh : meshes) {
		
		mesh.cleanUp();
	}
}

void weighedModel::load(std::string filepath) {
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, ASSIMP_LOAD_FLAGS);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		
		std::cout << "weighedModel::error : could not load model @" << filepath << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
	}
	
	directory = filepath.substr(0, filepath.find_last_of("/"));

	processScene(scene);
}

void weighedModel::processScene(const aiScene* scene) {
	
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		
		aiMesh* mesh = scene->mMeshes[i];

		// resizing vertexMap
		vertexMap.clear();
		vertexMap = std::vector<std::vector<std::pair<int, float> >> (mesh->mNumVertices);

		// processing bones
		if (mesh->HasBones()) {
			
			// resizing boneMap
			boneMap.clear();
			boneMap = std::vector<std::string> (mesh->mNumBones);

			for (unsigned int j = 0; j < mesh->mNumBones; ++j) {

				processBone(j, mesh->mBones[j]);
			}
		} else {
		
			// output warning
			std::cout << "weightModel::warning : provided mesh does not have an armature" << std::endl;
		}

		meshes.push_back(processMesh(mesh));
	}
}

weighedMesh weighedModel::processMesh(aiMesh* mesh) {
	
	std::vector<weighedVertex> vertices;
	std::vector<unsigned int> indices;

	// processing vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

		weighedVertex vertex;

		// position
		vertex.position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		// normals
		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);
		
		// color
		vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);

		// weights
		for (unsigned int j = 0; j < MAX_BONES_PER_VERTEX; ++j) {
			
			if (vertexMap.at(i).size() > j) {
				
				vertex.wvBoneData[j] = vertexMap.at(i).at(j).first;

				vertex.weightData[j] = vertexMap.at(i).at(j).second;
			} else {
				
				vertex.wvBoneData[j] = (-1);
				vertex.weightData[j] = (-1.0f);
			}
		}
		
		vertices.push_back(vertex);
	}

	// processing indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {

			indices.push_back(face.mIndices[j]);
		}
	}

	return weighedMesh(vertices, indices);
}

void weighedModel::processBone(unsigned int boneID, const aiBone* bone) {
	
	// updating boneMap
	boneMap.at(boneID) = bone->mName.C_Str();
	
	for (unsigned int i = 0; i < bone->mNumWeights; ++i) {

		// updating vertexMap
		vertexMap.at(bone->mWeights[i].mVertexId).push_back(std::pair<float, float> ({boneID, bone->mWeights[i].mWeight}));
	}
}
