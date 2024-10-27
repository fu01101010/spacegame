#ifndef VLIGHTSOURCE_HPP
#define VLIGHTSOURCE_HPP

#include "vCube.hpp"
#include "../light.h"

class vLightSource : public vCube {

public:

	glm::vec3 lightColor;

	//Light strength
	uPointLight PointLight;

	vLightSource() {}
	vLightSource(
		glm::vec3 lightColor = glm::vec3(1.0f), 
		glm::vec3 ambient = glm::vec3(1.0f), 
		glm::vec3 diffuse = glm::vec3(1.0f), 
		glm::vec3 specular = glm::vec3(1.0f), 
		glm::vec3 attC = glm::vec3(1.0f, 0.07f, 0.032f),
		glm::vec3 position = glm::vec3(0.0f), 
		glm::vec3 size = glm::vec3(1.0f))
		:	lightColor(lightColor), 
			PointLight({position, attC, ambient, diffuse, specular}),
			vCube(material::white_plastic, position, size) {}

	void render(shader Shader) {
		//set Light color
		Shader.set3flt("lightColor", lightColor);

		vCube::render(Shader);
	}
};

#endif
