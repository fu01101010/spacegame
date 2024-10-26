#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

class pointLight {

public:
	char type;

	glm::vec3 position;

	//attenuation constants
	float k0, k1, k2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

protected:
	void render(shader Shader, int idx);
};

class directLight {
	
public:
	char type;

	glm::vec3 direction;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

protected:
	void render(shader Shader);
};

class spotLight {
	
public:
	char type;

	glm::vec4 position;
	glm::vec4 direction;
	
	float innerRad, outerRad;

	//attenuation constants
	float k0, k1, k2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

protected:
	void render(shader Shader, int idx);
};

/*
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
*/


#endif
