#include "weighedModel.h"
#include "../source/utility/math/mat.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs)



//
//
// static model class here
weighedModel::weighedModel(glm::vec3 position, glm::vec3 size, bool noTex)
	: position(position), size(size), noTex(noTex) {}


void weighedModel::init() {}


void weighedModel::render(shader Shader, float time) {
	
	// in seconds
	//float time = glfwGetTime();

	glm::mat4 model = glm::mat4(1.0f);

	// update transform
	model = glm::translate(model, position);
	model = glm::scale(model, size);

	// push to shader
	Shader.setmat4("model", model);
	Shader.set_flt("Material.reflectivity", 0.5f);
	
	// process transforms
	std::vector<aiMatrix4x4> transforms;
	
	populateBoneTransforms(transforms, time);

	for (unsigned int j = 0; j < transforms.size(); ++j) {
			
		std::string dummy("boneData[" + std::to_string(j) + "]");
		Shader.setmat4(dummy, transforms[j]);
	}

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
	
	scene = importer.ReadFile(filepath, ASSIMP_LOAD_FLAGS);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		
		std::cout << "weighedModel::error : could not load model @" << filepath << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
	}
	
	// will set up later when standalone mat4 datatype will be avaliable
	// now in the form of namespace with a few funcs will upgrade much later

	globalInverseTransform = scene->mRootNode->mTransformation;
	globalInverseTransform = mat::inverse(globalInverseTransform);
	
	directory = filepath.substr(0, filepath.find_last_of("/"));

	currentAnim = 0;

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

	std::vector<texture> textures;

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

		// textureCoords
		if (mesh->mTextureCoords[0]) {
			
			vertex.textureCoords = glm::vec2(
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				);
		} else {
			
			vertex.textureCoords = glm::vec2(0.0f);
		}

		// weights
		for (unsigned int j = 0; j < MAX_BONES_PER_VERTEX; ++j) {
			
			if (vertexMap.at(i).size() > j) {
				
				vertex.wvBoneData[j] = vertexMap.at(i).at(j).first;

				vertex.weightData[j] = vertexMap.at(i).at(j).second;
			} else {
				
				vertex.wvBoneData[j] = (0);
				vertex.weightData[j] = (0.0f);
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

	// processing materials
	if (mesh->mMaterialIndex >= 0) {
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (noTex) {
			
			aiColor4D diffMaps(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffMaps);

			aiColor4D specMaps(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specMaps);

			return weighedMesh(vertices, indices, diffMaps, specMaps);
		}

		//diffMaps
		std::vector<texture> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());

		//specMaps
		std::vector<texture> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specMaps.begin(), specMaps.end());
	}

	return weighedMesh(vertices, indices, textures);
}


std::vector<texture> weighedModel::loadMaterialTextures(aiMaterial* material, aiTextureType type) {

	std::vector<texture> textures;

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
			texture tex(directory, helperString.C_Str(), type);
			
			tex.load();
			textures.push_back(tex);
			texturesLoaded.push_back(tex);
		}
	}

	return textures;
}


void weighedModel::processBone(unsigned int boneID, const aiBone* bone) {
	
	// updating boneMap
	boneMap.at(boneID) = bone->mName.C_Str();

	if (boneID == boneData.size()) {
		
		boneDataStruct bDSDummy(bone->mOffsetMatrix);
		boneData.push_back(bDSDummy);
	}
	
	for (unsigned int i = 0; i < bone->mNumWeights; ++i) {

		// updating vertexMap
		vertexMap.at(bone->mWeights[i].mVertexId).push_back(std::pair<float, float> ({boneID, bone->mWeights[i].mWeight}));
	}
}


void weighedModel::populateBoneTransforms(std::vector<aiMatrix4x4>& boneTransforms, float time) {
	
	// create identity matrix (identity provided by default constructor)
	aiMatrix4x4 identity;

	// calculate ticks per second
	float ticksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float timeInTicks = time * ticksPerSecond;
	float animTimeInTicks = std::fmod(timeInTicks, (float)scene->mAnimations[0]->mDuration);
	
	boneTransforms.resize(boneData.size());
	
	readNodeHierarchy(scene->mRootNode, identity, animTimeInTicks);

	for (unsigned int i = 0; i < boneData.size(); ++i) {
		
		boneTransforms[i] = boneData[i].endTransform;
	}
}


void weighedModel::readNodeHierarchy(const aiNode* node, const aiMatrix4x4& parentTransform, float time) {
	
	std::string name = node->mName.data;

	const aiAnimation* animation = scene->mAnimations[currentAnim];
	const aiNodeAnim* nodeAnim = findNodeAnim(animation, name);

	aiMatrix4x4 nodeTransform(node->mTransformation);

	if (nodeAnim) {
		
		// interpolate scaling and generate scaling transform
		aiVector3D scaling;
		calculateInterpolatedScaling(scaling, time, nodeAnim);
		glm::mat4 interpolatedScaling = glm::mat4(1.0f);
		interpolatedScaling = scale(interpolatedScaling, glm::vec3(scaling.x, scaling.y, scaling.z));

		// interpolate rotation and generate rotation transform
		aiQuaternion rotation;
		calculateInterpolatedRotation(rotation, time, nodeAnim);
		aiMatrix4x4 interpolatedRotation(rotation.GetMatrix());

		// interpolate translation and generate translation transform
		aiVector3D translation;
		calculateInterpolatedTranslation(translation, time, nodeAnim);
		glm::mat4 interpolatedTranslation = glm::mat4(1.0f);
		interpolatedTranslation = translate(interpolatedTranslation, glm::vec3(translation.x, translation.y, translation.z));

		nodeTransform = mat::GLMToAI(interpolatedTranslation) * interpolatedRotation * mat::GLMToAI(interpolatedScaling);
	}

	//printf("%s - ", name.c_str());

	aiMatrix4x4 globalTransform = parentTransform * nodeTransform;
	
	for (unsigned int i = 0; i < boneMap.size(); ++i) {
		
		if (boneMap[i] == name) {
			
			// add inverse global transform here when done
			boneData[i].endTransform = globalInverseTransform * globalTransform * boneData[i].offsetMatrix;
			break;
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		
		readNodeHierarchy(node->mChildren[i], globalTransform, time);
	}
}


const aiNodeAnim* weighedModel::findNodeAnim(const aiAnimation* animation, const std::string name) {
	
	for (unsigned int i = 0; i < animation->mNumChannels; ++i) {
		
		const aiNodeAnim* nodeAnim = animation->mChannels[i];

		if (std::string(nodeAnim->mNodeName.data) == name) {
			
			return nodeAnim;
		}
	}

	return nullptr;
}


unsigned int weighedModel::findTranslationIndex(float time, const aiNodeAnim* nodeAnim) {
	
	assert(nodeAnim->mNumPositionKeys > 0);

	for (unsigned int i = 1; i < nodeAnim->mNumPositionKeys - 1; ++i) {
		
		float tick = (float)nodeAnim->mPositionKeys[i + 1].mTime;

		if (time < tick) {

			return i;
		}
	}

	return 0;
}


void weighedModel::calculateInterpolatedTranslation(aiVector3D& translation, float time, const aiNodeAnim* nodeAnim) {
	
	// at least two keys are needed to interpolate
	if (nodeAnim->mNumPositionKeys == 1) {
		
		translation = nodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int translationIndex = findTranslationIndex(time, nodeAnim);
	unsigned int nextTranslationIndex = translationIndex + 1;

	assert(nextTranslationIndex < nodeAnim->mNumPositionKeys);

	float tick1 = (float)nodeAnim->mPositionKeys[translationIndex].mTime;
	float tick2 = (float)nodeAnim->mPositionKeys[nextTranslationIndex].mTime;

	float deltaTime = tick2 - tick1;

	float factor = (time - tick1) / deltaTime;

	// ???
	// sometimes negative and less than ~-1.0f
	//assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = nodeAnim->mPositionKeys[translationIndex].mValue;
	const aiVector3D& end = nodeAnim->mPositionKeys[nextTranslationIndex].mValue;

	aiVector3D delta = end - start;

	translation = start + factor * delta;
}


unsigned int weighedModel::findRotationIndex(float time, const aiNodeAnim* nodeAnim) {
	
	assert(nodeAnim->mNumRotationKeys > 0);
	
	for (unsigned int i = 0 ; i < nodeAnim->mNumRotationKeys - 1 ; ++i) {
		
		float tick = (float)nodeAnim->mRotationKeys[i + 1].mTime;
		
		if (time < tick) {
			
			return i;
		}
	}
	
	return 0;
}


void weighedModel::calculateInterpolatedRotation(aiQuaternion& rotation, float time, const aiNodeAnim* nodeAnim) {
	
	// at least two keys are needed to interpolate
	if (nodeAnim->mNumRotationKeys == 1) {
		
		rotation = nodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int rotationIndex = findRotationIndex(time, nodeAnim);
	unsigned int nextRotationIndex = rotationIndex + 1;

	assert(nextRotationIndex < nodeAnim->mNumRotationKeys);

	float tick1 = (float)nodeAnim->mRotationKeys[rotationIndex].mTime;
	float tick2 = (float)nodeAnim->mRotationKeys[nextRotationIndex].mTime;

	float deltaTime = tick2 - tick1;

	float factor = (time - tick1) / deltaTime; 

	// ???
	// sometimes negative and less than ~-1.0f
	//assert(factor >= 0.0f && factor <= 1.0f);

	const aiQuaternion& start = nodeAnim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& end = nodeAnim->mRotationKeys[nextRotationIndex].mValue;

	aiQuaternion::Interpolate(rotation, start, end, factor);

	rotation.Normalize();	
}


unsigned int weighedModel::findScalingIndex(float time, const aiNodeAnim* nodeAnim) {
	
	assert(nodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 1; i < nodeAnim->mNumScalingKeys - 1; ++i) {
		
		float tick = (float)nodeAnim->mScalingKeys[i + 1].mTime;

		if (time < tick) {

			return i;
		}
	}

	return 0;
}


void weighedModel::calculateInterpolatedScaling(aiVector3D& scaling, float time, const aiNodeAnim* nodeAnim) {
	
	// at least two keys are needed to interpolate
	if (nodeAnim->mNumScalingKeys == 1) {
		
		scaling = nodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int scalingIndex = findScalingIndex(time, nodeAnim);
	unsigned int nextScalingIndex = scalingIndex + 1;

	assert(nextScalingIndex < nodeAnim->mNumScalingKeys);

	float tick1 = (float)nodeAnim->mScalingKeys[scalingIndex].mTime;
	float tick2 = (float)nodeAnim->mScalingKeys[nextScalingIndex].mTime;

	float deltaTime = tick2 - tick1;

	float factor = (time - tick1) / deltaTime;
	
	// ???
	// sometimes negative and less than ~-1.0f
	//assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = nodeAnim->mScalingKeys[scalingIndex].mValue;
	const aiVector3D& end = nodeAnim->mScalingKeys[nextScalingIndex].mValue;

	aiVector3D delta = end - start;

	scaling = start + factor * delta;
}
