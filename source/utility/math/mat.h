#ifndef MAT_H
#define MAT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>

namespace mat {
	
	aiMatrix4x4 inverse(aiMatrix4x4& m);

	glm::mat4 AIToGLM(aiMatrix4x4 m);

	aiMatrix4x4 GLMToAI(glm::mat4 m);

	void print(glm::mat4 m);

	void print(aiMatrix4x4 m);
}

#endif
