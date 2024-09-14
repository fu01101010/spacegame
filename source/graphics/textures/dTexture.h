#ifndef DTEXTURE_H
#define DTEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/scene.h>

#include <stb/stb_image.h>

#include <string>

class dTexture {

public:

	dTexture();
	dTexture(std::string directory, std::string path, aiTextureType type);

	void generate();
	void load(bool flip = true);

	void bind();

	//texture object
	unsigned int id;
	aiTextureType type;

	std::string directory;
	std::string path;
};

#endif
