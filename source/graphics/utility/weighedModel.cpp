#include "weighedMesh.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices)

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
	
	for (weighedMesh mesh : mesh) {
		
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
		
		const aiMesh* mesh = scene->mMeshes[i];

		// resizing vertexMap
		vertexMap.clear();
		vertexMap.resize(mesh->mNumVertices);

		// processing bones
		if (mesh->HasBones()) {
			
			// resizing boneMap
			boneMap.clear();
			boneMap.resize(mesh->mNumBones);

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

weighedMesh processMesh(const aiMesh* mesh) {
	
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

		// weights
		for (unsigned int j = 0; j < MAX_BONES_PER_VERTEX; ++j) {
			
			vertex.weightData.at(j) = vertexMap.at(i).at(j);	
		}
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

void processBone(unsigned int boneID, const aiBone* bone) {
	
	// updating boneMap
	boneMap.at(i) = bone->mName.C_Str();
	
	for (int i = 0; i < bone->mNumWeights; ++i) {
		
		// adding new vertex if empty
		if (vertexMap.at(mWeights[i].mVertexId).size() == 0) {
			
			std::vector<float> initDummy(MAX_BONES_PER_VERTEX, 0.0f);

			vertexMap.at(mWeights[i].mVertexId) = initDummy;
		}

		// updating vertexMap
		vertexMap.at(mWeights[i].mVertexId).at(boneID) = mWeights[i].mWeight;
	}
}
