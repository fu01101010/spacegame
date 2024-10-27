#include "light.h"

pointLight::pointLight(char type, glm::vec3 position, float k0, float k1, float k2, 
		glm::vec3 ambient, 
		glm::vec3 diffuse, 
		glm::vec3 specular) 
{}

pointLight::pointLight(char type, glm::vec3 position, float k0, float k1, float k2, 
		glm::vec4 ambient, 
		glm::vec4 diffuse, 
		glm::vec4 specular) 
{}

directLight::directLight(char type, glm::vec3 direction, 
		glm::vec3 ambient, 
		glm::vec3 diffuse, 
		glm::vec3 specular) 
{}

directLight::directLight(char type, glm::vec3 direction, 
		glm::vec4 ambient, 
		glm::vec4 diffuse, 
		glm::vec4 specular) 
{}

spotLight::spotLight(char type, glm::vec3 position, glm::vec3 direction, float innerRad, float outerRad, float k0, float k1, float k2, 
		glm::vec3 ambient, 
		glm::vec3 diffuse, 
		glm::vec3 specular) 
{}

spotLight::spotLight(char type, glm::vec3 position, glm::vec3 direction, float innerRad, float outerRad, float k0, float k1, float k2, 
		glm::vec3 ambient, 
		glm::vec3 diffuse, 
		glm::vec3 specular) 
{}

void pointLight::render(shader Shader, int idx) {
	
	switch (type) {
		
		case ('m'): {
			
			
		}
	}
	std::string name = "PointLights[" + std::to_string(IDx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set_flt(name + ".k0", k0);
	Shader.set_flt(name + ".k1", k1);
	Shader.set_flt(name + ".k2", k2);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void directLight::render(shader Shader) {

	std::string name = "DirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}


void spotLight::render(shader Shader) {

	//std::string name = "SpotLights[" + std::to_string(IDx) + "]";
	std::string name = "SpotLight";

	Shader.set3flt(name + ".position", position);
	Shader.set3flt(name + ".direction", direction);
	
	Shader.set_flt(name + ".cutOff", cutOff);
	Shader.set_flt(name + ".outerCutOff", outerCutOff);

	Shader.set_flt(name + ".k0", k0);
	Shader.set_flt(name + ".k1", k1);
	Shader.set_flt(name + ".k2", k2);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}
