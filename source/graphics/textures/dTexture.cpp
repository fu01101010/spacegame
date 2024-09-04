#include <iostream>

#include "DFLT_Texture.h"

DFLT_TEXTURE::DFLT_TEXTURE() {}
DFLT_TEXTURE::DFLT_TEXTURE(std::string DIRECTORY, std::string PATH, aiTextureType type)
	: DIRECTORY(DIRECTORY), PATH(PATH), type(type){}

void DFLT_TEXTURE::GENERATE() {

	glGenTextures(1, &ID);
	//glBindTexture(GL_TEXTURE_2D, ID??);
}

void DFLT_TEXTURE::LOAD(bool Flip) {

	stbi_set_flip_vertically_on_load(Flip);

	int Width, Height, NChannels;

	unsigned char* DATA = stbi_load((DIRECTORY + "/" + PATH).c_str(), &Width, &Height, &NChannels, 0);

	GLenum ColorMode;
	switch (NChannels) {

	case 1: {
		ColorMode = GL_RED;
		break;
	}

	case 4: {
		ColorMode = GL_RGBA;
		break;
	}

	default:
		ColorMode = GL_RGB;
	}

	if (DATA) {

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, ColorMode, Width, Height, 0, ColorMode, GL_UNSIGNED_BYTE, DATA);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {

		std::cout << "Failed to load image at" << PATH << std::endl;
	}

	stbi_image_free(DATA);
}

void DFLT_TEXTURE::BIND() {

	glBindTexture(GL_TEXTURE_2D, ID);
}