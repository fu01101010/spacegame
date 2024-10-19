#ifndef VTEXTURE_H
#define VTEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

class vTexture {

public:

	vTexture();
	vTexture(const char* path, const char* name, bool defaultParameters = true);

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

#endif
