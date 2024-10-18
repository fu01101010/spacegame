#ifndef VMODEL_NOTEX_H
#define VMODEL_NOTEX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../vMesh_notex.h"

class vModel_notex {

public:

	std::vector<vMesh_notex> vMeshes;

	vModel_notex();

	void init();

	void render(shader Shader);

	void cleanUp();
};

#endif
