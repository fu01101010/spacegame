#ifndef VLIGHTSOURCE_HPP
#define VLIGHTSOURCE_HPP

#include "vcube.hpp"
#include "../light.h"

class vlightsource : public vcube {

public:

	glm::vec3 lightColor;

	//Light strength
	old_pointLight PointLight;

	vlightsource() {}
	vlightsource(
		glm::vec3 lightColor = glm::vec3(1.0f), 
		glm::vec3 ambient = glm::vec3(1.0f), 
		glm::vec3 diffuse = glm::vec3(1.0f), 
		glm::vec3 specular = glm::vec3(1.0f), 
		float k0 = 1.0f, 
		float k1 = 0.09f, 
		float k2 = 0.032f,
		glm::vec3 position = glm::vec3(0.0f), 
		glm::vec3 size = glm::vec3(1.0f))
		:	lightColor(lightColor), 
			PointLight({position, k0, k1, k2, ambient, diffuse, specular}),
			vcube(material::white_plastic, position, size) {}

	void render(shader Shader) {
		//set Light color
		Shader.set3flt("lightColor", lightColor);

		vcube::render(Shader);
	}
};

#endif
