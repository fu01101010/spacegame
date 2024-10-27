#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <typeinfo>

#include "shader.h"

template< typename glm__vecN > class pointLight {

public:
	// basic constructor for now
	pointLight(glm::vec3 position, glm::vec3 attC, glm__vecN ambient, glm__vecN diffuse, glm__vecN specular);
	
	glm::vec3 position;

	// attenuation constants
	// k0 = attDat.x ... k2 = attDat.z
	glm::vec3 attC;	

	glm__vecN ambient;
	glm__vecN diffuse;
	glm__vecN specular;

	void render(shader Shader, int idx);
};

template< typename glm__vecN > class directLight {
	
public:
	// basic constructor for now
	directLight(glm::vec3 direction, glm__vecN ambient, glm__vecN diffuse, glm__vecN specular);

	glm::vec3 direction;

	glm__vecN ambient;
	glm__vecN diffuse;
	glm__vecN specular;

	void render(shader Shader);
};

template< typename glm__vecN > class spotLight {
	
public:
	// basic constructor for now
	spotLight(glm::vec3 position, glm::vec3 direction, float innR, float outR, glm::vec3 attC, glm__vecN ambient, glm__vecN diffuse, glm__vecN specular);
	
	glm::vec3 position;
	glm::vec3 direction;
	
	// innerRadius, outerRadius
	float innR, outR;

	// attenuation constants
	// k0 = attDat.x ... k2 = attDat.z
	glm::vec3 attC;

	glm__vecN ambient;
	glm__vecN diffuse;
	glm__vecN specular;

	void render(shader Shader, int idx);
};

#endif
