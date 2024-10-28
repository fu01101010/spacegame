#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

<<<<<<< HEAD
struct uPointLight {
=======
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
>>>>>>> 528a8ee5ae6310d1daa5483bac7eb887feeaa601

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
*/


#endif
