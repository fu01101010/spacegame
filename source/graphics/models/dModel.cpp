#include "dModel.h"

dModel::dModel() {}
dModel::dModel(glm::vec3 position, glm::vec3 size)
	: position(position), size(size) { }

void dModel::init() {}

void dModel::render(shader Shader) {

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::scale(model, size);

	Shader.setmat4("model", model);
	Shader.set_flt("Material.reflectivity", 0.5f);

	for (dMesh mesh : meshes) {

		mesh.render(Shader);
	}
}

void dModel::cleanUp() {

	for (dMesh mesh : meshes) {

		mesh.cleanUp();
	}
}

void dModel::loadModel(std::string path) {

	Assimp::Importer Importer;
	const aiScene* scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

		std::cout << "Could not load model at " << path << std::endl << Importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);
}

void dModel::processNode(aiNode* node, const aiScene* scene) {

	//process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		meshes.push_back(processMesh(mesh, scene));
	}

	//process all child nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {

		processNode(node->mChildren[i], scene);
	}
}

dMesh dModel::processMesh(aiMesh* mesh, const aiScene* scene) {

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<dTexture> textures;

	//vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

		vertex Vertex;

		//position
		Vertex.position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		//normals
		Vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		//textures
		if (mesh->mTextureCoords[0]) {

			Vertex.texCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else {

			Vertex.texCoord = glm::vec2(0.0f);
		}

		vertices.push_back(Vertex);
	}

	//Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {

		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {

			indices.push_back(face.mIndices[j]);
		}
	}

	//Process material
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//diffMaps
		std::vector<dTexture> diffMaps = loadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());

		//specMaps
		std::vector<dTexture> specMaps = loadTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specMaps.begin(), specMaps.end());
	}

	return dMesh(vertices, indices, textures);
}

std::vector<dTexture> dModel::loadTextures(aiMaterial* material, aiTextureType type) {

	std::vector<dTexture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {

		aiString helperString;

		material->GetTexture(type, i, &helperString);
		std::cout << helperString.C_Str() << std::endl;

		//prevent duplicate loading
		bool helperSkipFlag = false;

		for (unsigned int j = 0; j < texturesLoaded.size(); ++j) {

			if (std::strcmp(texturesLoaded[j].path.data(), helperString.C_Str())) {

				textures.push_back(texturesLoaded[j]);

				helperSkipFlag = true;
				break;
			}
		}

		if (!helperSkipFlag) {

			//not loaded yet
			dTexture tex(directory, helperString.C_Str(), type);
			tex.load(false);

			textures.push_back(tex);
			texturesLoaded.push_back(tex);
		}
	}

	return textures;
}
