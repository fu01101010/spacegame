#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <assimp/scene.h>

#include <string>


class v_texture {

public:

	v_texture();
	v_texture(const char* path, const char* name, bool defaultParameters = true);

	void generate();
	void load(bool flip = true);

	void setFilters(GLenum all);
	void setFilters(GLenum mag, GLenum min);

	void setWrap(GLenum all);
	void setWrap(GLenum s, GLenum t);
	
	void setBorderColor(float borderColor[4]);

	void activate();

	// tex object
	int texID;
	unsigned tex;
	const char* texName;

private:

	static int currentID;

	const char* texPath;
	int width;
	int height;
	int nChannels;
};


class m_texture {

public:

	m_texture();
	m_texture(std::string directory, std::string path, aiTextureType type);

	void generate();
	void load();

	void bind();

	//texture object
	unsigned int id;
	aiTextureType type;

	std::string directory;
	std::string path;
};






//
//
// normal texture class

class texture {

public:

	texture();
	texture(std::string directory, std::string path, aiTextureType type);

	void generate();
	void load();

	void bind();

	//texture object
	unsigned int id;
	aiTextureType type;

	std::string directory;
	std::string path;
};

#endif
