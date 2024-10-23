#ifndef DLIGHT_H
#define DLIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

struct dPointLight {

	glm::vec3 position;

	//attenuation constants
	float k0;
	float k1;
	float k2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader, int IDx);
};

struct dDirectLight {

	glm::vec3 direction;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader);
};

struct dSpotLight {

	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;

	//attenuation constants
	float k0;
	float k1;
	float k2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader);
};

#endif
