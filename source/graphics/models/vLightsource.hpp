#ifndef VERT_LIGHTSOURCE_HPP
#define VERT_LIGHTSOURCE_HPP

#include "VERT_cube.hpp"
#include "../../Light.h"

class VERT_LIGHTSOURCE : public VERT_CUBE {

public:

	glm::vec3 LightColor;

	//LIGHT strength
	POINTLIGHT PointLight;

	VERT_LIGHTSOURCE() {}
	VERT_LIGHTSOURCE(
		glm::vec3 LightColor, 
		glm::vec3 Ambient, glm::vec3 Diffuse, glm::vec3 Specular, 
		float k0, float k1, float k2,
		glm::vec3 Position, glm::vec3 Size)
		:	LightColor(LightColor), 
			PointLight({Position, k0, k1, k2, Ambient, Diffuse, Specular}),
			VERT_CUBE(MATERIAL::white_plastic, Position, Size) {}

	void RENDER(SHADER Shader) {
		//SET LIGHT color
		Shader.SET3FLT("LightColor", LightColor);

		VERT_CUBE::RENDER(Shader);
	}
};

#endif