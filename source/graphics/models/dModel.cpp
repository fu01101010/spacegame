#include "DFLT_Model.h"

DFLT_MODEL::DFLT_MODEL() {}
DFLT_MODEL::DFLT_MODEL(glm::vec3 Position, glm::vec3 Size)
	: Position(Position), Size(Size) { }

void DFLT_MODEL::INIT() {}

void DFLT_MODEL::RENDER(SHADER SHADER) {

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, Position);
	model = glm::scale(model, Size);

	SHADER.SETMAT4("MODEL", model);
	SHADER.SET_FLT("Material.Reflectivity", 0.5f);

	for (DFLT_MESH MESH : MESHES) {

		MESH.RENDER(SHADER);
	}
}

void DFLT_MODEL::CleanUp() {

	for (DFLT_MESH MESH : MESHES) {

		MESH.CleanUp();
	}
}

void DFLT_MODEL::LOADModel(std::string PATH) {

	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(PATH, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {

		std::cout << "Could not load model at " << PATH << std::endl << Importer.GetErrorString() << std::endl;
		return;
	}

	DIRECTORY = PATH.substr(0, PATH.find_last_of("/"));

	ProcessNODE(Scene->mRootNode, Scene);
}

void DFLT_MODEL::ProcessNODE(aiNode* node, const aiScene* Scene) {

	//process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {

		aiMesh* mesh = Scene->mMeshes[node->mMeshes[i]];

		MESHES.push_back(ProcessMESH(mesh, Scene));
	}

	//process all child nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {

		ProcessNODE(node->mChildren[i], Scene);
	}
}

DFLT_MESH DFLT_MODEL::ProcessMESH(aiMesh* mesh, const aiScene* Scene) {

	std::vector<VERTEX> Vertices;
	std::vector<unsigned int> Indices;

	std::vector<DFLT_TEXTURE> Textures;

	//Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

		VERTEX Vertex;

		//Position
		Vertex.Position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		//Normals
		Vertex.Normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		//Textures
		if (mesh->mTextureCoords[0]) {

			Vertex.TexCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else {

			Vertex.TexCoord = glm::vec2(0.0f);
		}

		Vertices.push_back(Vertex);
	}

	//Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {

		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {

			Indices.push_back(face.mIndices[j]);
		}
	}

	//Process material
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* material = Scene->mMaterials[mesh->mMaterialIndex];

		//diffMaps
		std::vector<DFLT_TEXTURE> diffMaps = LoadTextures(material, aiTextureType_DIFFUSE);
		Textures.insert(Textures.end(), diffMaps.begin(), diffMaps.end());

		//specMaps
		std::vector<DFLT_TEXTURE> specMaps = LoadTextures(material, aiTextureType_SPECULAR);
		Textures.insert(Textures.end(), specMaps.begin(), specMaps.end());
	}

	return DFLT_MESH(Vertices, Indices, Textures);
}

std::vector<DFLT_TEXTURE> DFLT_MODEL::LoadTextures(aiMaterial* material, aiTextureType type) {

	std::vector<DFLT_TEXTURE> Textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {

		aiString helperString;

		material->GetTexture(type, i, &helperString);
		std::cout << helperString.C_Str() << std::endl;

		//prevent duplicate loading
		bool helperSkipFlag = false;

		for (unsigned int j = 0; j < TexturesLoaded.size(); ++j) {

			if (std::strcmp(TexturesLoaded[j].PATH.data(), helperString.C_Str())) {

				Textures.push_back(TexturesLoaded[j]);

				helperSkipFlag = true;
				break;
			}
		}

		if (!helperSkipFlag) {

			//not loaded yet
			DFLT_TEXTURE TEX(DIRECTORY, helperString.C_Str(), type);
			TEX.LOAD(false);

			Textures.push_back(TEX);
			TexturesLoaded.push_back(TEX);
		}
	}

	return Textures;
}