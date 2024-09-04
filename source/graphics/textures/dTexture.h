#ifndef DFLT_TEXTURE_H
#define DFLT_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/scene.h>

#include <stb/stb_image.h>

class DFLT_TEXTURE {

public:

	DFLT_TEXTURE();
	DFLT_TEXTURE(std::string DIRECTORY, std::string PATH, aiTextureType type);

	void GENERATE();
	void LOAD(bool Flip = true);

	void BIND();

	unsigned int ID;
	aiTextureType type;

	std::string DIRECTORY;
	std::string PATH;
};

#endif