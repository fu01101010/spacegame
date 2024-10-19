#include <iostream>

#include "vTexture.h"

int vTexture::currentID = 0;

vTexture::vTexture() : texPath(""), texName(""), texID(-1), width(0), height(0), nChannels(0) {}
vTexture::vTexture(const char* path, const char* name, bool defaultParameters)
	: texPath(path), texName(name), texID(currentID++) {

	generate();

	if (defaultParameters) {

		setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		setWrap(GL_REPEAT);
	}
}

void vTexture::generate() {

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void vTexture::load(bool flip) {

	stbi_set_flip_vertically_on_load(flip);

	unsigned char* data = stbi_load(texPath, &width, &height, &nChannels, 0);

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

		default :
			colorMode = GL_RGB;
	}

	if (data) {

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D); 
	}
	else {

		std::cout << "Failed to load image@ " << texPath << std::endl;
	}

	stbi_image_free(data);
}

void vTexture::setFilters(GLenum all) {

	setFilters(all, all);
}

void vTexture::setFilters(GLenum mag, GLenum min) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
}

void vTexture::setWrap(GLenum all) {

	setWrap(all, all);
}

void vTexture::setWrap(GLenum s, GLenum t) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void vTexture::activate() {

	glActiveTexture(GL_TEXTURE0 + texID);
}

void vTexture::setBorderColor(float borderColor[4]) {
	
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}
