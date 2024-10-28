#include <iostream>

#include "texture.h"



//
//
// vertex texture

int v_texture::currentID = 0;

v_texture::v_texture() : texPath(""), texName(""), texID(-1), width(0), height(0), nChannels(0) {}
v_texture::v_texture(const char* path, const char* name, bool defaultParameters)
	: texPath(path), texName(name), texID(currentID++) {

	generate();

	if (defaultParameters) {

		setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		setWrap(GL_REPEAT);
	}
}

void v_texture::generate() {

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void v_texture::load(bool flip) {

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

void v_texture::setFilters(GLenum all) {

	setFilters(all, all);
}

void v_texture::setFilters(GLenum mag, GLenum min) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
}

void v_texture::setWrap(GLenum all) {

	setWrap(all, all);
}

void v_texture::setWrap(GLenum s, GLenum t) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void v_texture::activate() {

	glActiveTexture(GL_TEXTURE0 + texID);
}

void v_texture::setBorderColor(float borderColor[4]) {
	
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}



//
//
// model texture

m_texture::m_texture()
	: directory(""), path(""){}
	
m_texture::m_texture(std::string directory, std::string path, aiTextureType type)
	: directory(directory), path(path), type(type) {

	generate();	
}

void m_texture::generate() {

	glGenTextures(1, &id);
	//glBindTexture(GL_TEXTURE_2D, id??);
}

void m_texture::load() {

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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {

		std::cout << "Failed to load image at " << path << std::endl;
	}

	stbi_image_free(data);
}

void m_texture::bind() {

	glBindTexture(GL_TEXTURE_2D, id);
}
