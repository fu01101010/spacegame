#include "model.h"



//
//
// default vertex model

u_model::u_model() {}

void u_model::init() {}

void u_model::render(shader Shader) {

	for (mesh mesh : meshes) {

		mesh.render(Shader);
	}
}

void u_model::cleanUp() {

	for (mesh mesh : meshes) {

		mesh.cleanUp();
	}
}



//
//
// .gltf model

m_model::m_model(glm::vec3 position, glm::vec3 size, bool noTex)
	: position(position), size(size), noTex(noTex) { }

void m_model::init() {}

void m_model::render(shader Shader) {

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::scale(model, size);
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	Shader.setmat4("model", model);
	Shader.set_flt("Material.reflectivity", 0.5f);

	for (mesh mesh : meshes) {

		mesh.render(Shader);
	}
}

void m_model::cleanUp() {

	for (mesh mesh : meshes) {

		mesh.cleanUp();
	}
}

void m_model::loadModel(std::string path) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

		std::cout << "Could not load model at " << path << std::endl << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);
}

void m_model::processNode(aiNode* node, const aiScene* scene) {

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

mesh m_model::processMesh(aiMesh* mesh, const aiScene* scene) {

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<m_texture> textures;

	//vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

		vertex vertex;

		//position
		vertex.position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		//normals
		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		//textures
		if (mesh->mTextureCoords[0]) {

			vertex.texCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else {

			vertex.texCoord = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
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

		if (noTex) {

			aiColor4D diffMaps(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffMaps);

			aiColor4D specMaps(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specMaps);

			return mesh(vertices, indices, diffMaps, specMaps);
		}


		//diffMaps
		std::vector<m_texture> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());

		//specMaps
		std::vector<m_texture> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specMaps.begin(), specMaps.end());
	}

	return mesh(vertices, indices, textures);
}

std::vector<m_texture> m_model::loadMaterialTextures(aiMaterial* material, aiTextureType type) {

	std::vector<m_texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {

		aiString helperString;

		material->GetTexture(type, i, &helperString);
		std::cout << helperString.C_Str() << std::endl;

		//prevent duplicate loading
		bool helperSkipFlag = false;

		for (unsigned int j = 0; j < texturesLoaded.size(); ++j) {

			if (std::strcmp(texturesLoaded[j].path.data(), helperString.C_Str()) == 0) {

				textures.push_back(texturesLoaded[j]);

				helperSkipFlag = true;
				break;
			}
		}

		if (!helperSkipFlag) {

			std::cout << directory << "///" << helperString.C_Str() << std::endl;

			//not loaded yet
			dTexture tex(directory, helperString.C_Str(), type);
			
			tex.load();
			textures.push_back(tex);
			texturesLoaded.push_back(tex);
		}
	}

	return textures;
}
