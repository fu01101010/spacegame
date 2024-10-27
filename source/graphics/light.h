#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

struct uPointLight {

	glm::vec3 position;

	//attenuation constants
	//k0 <- attC.x ... k2 <- attC.z
	glm::vec3 attC;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(shader Shader, int idx);
};

struct mPointLight {

	glm::vec3 position;

	//attenuation constants
	//k0 <- attC.x ... k2 <- attC.z
	glm::vec3 attC;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader, int idx);
};

struct uDirectLight {

	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(shader Shader);
};

struct mDirectLight {

	glm::vec3 direction;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader);
};

struct uSpotLight {

	glm::vec3 position;
	glm::vec3 direction;

	// innR <- inner cut off angle
	// outR <- outer cut off angle
	float innR, outR;

	//attenuation constants
	//k0 <- attC.x ... k2 <- attC.z
	glm::vec3 attC;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader, int idx);
};

struct mSpotLight {

	glm::vec3 position;
	glm::vec3 direction;

	// innR <- inner cut off angle
	// outR <- outer cut off angle
	float innR, outR;

	//attenuation constants
	//k0 <- attC.x ... k2 <- attC.z
	glm::vec3 attC;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader, int idx);
};

#endif
