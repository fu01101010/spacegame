#include <iostream>

#include "vTexture.h"

int vTexture::Currentid;

vTexture::vTexture() {}
vTexture::vTexture(const char* path, const char* name, bool defaultParameters)
	: tex_path(path), tex_name(name), tex_id(currentID++) {

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

	stbi_set_flip_vertically_on_load(Flip);

	unsigned char* data = stbi_load(tex_path, &width, &height, &nChannels, 0);

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

		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D); 
	}
	else {

		std::cout << "Failed to load image@ " << tex_path << std::endl;
	}

	stbi_image_free(data);
}

void vTexture::setFilters(GLenum all) {

	setFilters(all, all);
}

void vTexture::setFilters(GLenum mag, GLenum min) {

	glTexParameteri(GL_texTURE_2D, GL_texTURE_mag_FILTER, mag);
	glTexParameteri(GL_texTURE_2D, GL_texTURE_min_FILTER, min);
}

void vTexture::setWrap(GLenum all) {

	setWrap(all, all);
}

void vTexture::setWrap(GLenum s, GLenum t) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void vTexture::bind() {

	glBindTexture(GL_TEXTURE_2D, tex_id);
}
