#include <iostream>

#include "dTexture.h"

dTexture::dTexture()
	: directory(""), path(""){}
	
dTexture::dTexture(std::string directory, std::string path, aiTextureType type)
	: directory(directory), path(path), type(type){

	generate();	
}

void dTexture::generate() {

	glGenTextures(1, &id);
	//glBindTexture(GL_TEXTURE_2D, id??);
}

void dTexture::load(bool flip) {

	int width, height, nChannels;

	unsigned char* data = stbi_load((directory + "/" + path).c_str(), &width, &height, &nChannels, 0);

	GLenum colorMode;
	switch (nChannels) {

	case 1: {
		colorMode = GL_RED;
		break;
	}

	case 4: {
		colorMode = GL_RGBA;
		break;
	}

	default:
		colorMode = GL_RGB;
	}

	if (data) {

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {

		std::cout << "Failed to load image at" << path << std::endl;
	}

	stbi_image_free(data);
}

void dTexture::bind() {

	glBindTexture(GL_TEXTURE_2D, id);
}
