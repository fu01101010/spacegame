#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"



//
//
// glm::vec3 !
struct pointLight {

	glm::vec3 position;

	//attenuation constants
	float k0;
	float k1;
	float k2;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(shader Shader, int IDx);
};

struct directLight {

	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(shader Shader);
};

struct spotLight {

	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;

	//attenuation constants
	float k0;
	float k1;
	float k2;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(shader Shader);
};



//
//
// glm::vec4 !
struct m_pointLight {

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

struct m_directLight {

	glm::vec3 direction;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(shader Shader);
};

struct m_spotLight {

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
