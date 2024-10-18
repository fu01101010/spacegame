#ifndef VLIGHTSOURCE_HPP
#define VLIGHTSOURCE_HPP

#include "vCube.hpp"
#include "../light.h"

class vLightSource : public vCube {

public:

	glm::vec3 lightColor;

	//Light strength
	pointLight PointLight;

	vLightSource() {}
	vLightSource(
		glm::vec3 lightColor, 
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
		float k0, float k1, float k2,
		glm::vec3 position, glm::vec3 size)
		:	lightColor(lightColor), 
			PointLight({position, k0, k1, k2, ambient, diffuse, specular}),
			vCube(material::white_plastic, position, size) {}

	void render(shader Shader) {
		//set Light color
		Shader.set3flt("lightColor", lightColor);

		vCube::render(Shader);
	}
};

#endif
