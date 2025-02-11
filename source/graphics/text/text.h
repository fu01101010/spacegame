#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <string>
#include <map>

// #include "glmemory.hpp" // generalisations for opengl buffers management
#include "../shader.h"

typedef struct {
	
	unsigned int 	textureID; 	// textureID storing the character
	glm::ivec2 	size;		// size of the character
	glm::ivec2 	bearing;	// distance from origin to top left
	unsigned int 	advance;	// distance from origin to next origin
} character;

class ftText {
	
public:
	ftText();
	ftText(unsigned int height);

	bool load(FT_Library &ft, std::string path);

	void render(shader Shader, std::string text, float x, float y, glm::vec2 scale, glm::vec3 color);

	void cleanUp();

private:
	unsigned int height;

	std::map<char, character> characterMap;

	// update for scene!!!
	unsigned int VAO, VBO;
	//error
};

#endif
