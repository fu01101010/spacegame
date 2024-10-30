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

void PointLight::render(shader Shader, int idx) {
	
	std::string name = "uPointLights[" + std::to_string(IDx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set3flt(name + ".attC", attC);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void PointLight::render(shader Shader, int idx) {

	std::string name = "mPointLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set3flt(name + ".attC", attC);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}

void DirectLight::render(shader Shader) {

	std::string name = "uDirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void DirectLight::render(shader Shader) {

	std::string name = "mDirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}

void SpotLight::render(shader Shader, int idx) {

	std::string name = "uSpotLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);
	Shader.set3flt(name + ".direction", direction);
	
	Shader.set_flt(name + ".innR", innR);
	Shader.set_flt(name + ".outR", outR);

	Shader.set3flt(name + ".attC", attC);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void SpotLight::render(shader Shader, int idx) {

	std::string name = "mSpotLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);
	Shader.set3flt(name + ".direction", direction);
	
	Shader.set_flt(name + ".innR", innR);
	Shader.set_flt(name + ".outR", outR);

	Shader.set3flt(name + ".attC", attC);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}
