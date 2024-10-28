#include "light.h"

<<<<<<< HEAD
void uPointLight::render(shader Shader, int idx) {

	std::string name = "PointLights[" + std::to_string(idx) + "]";
=======
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
>>>>>>> 528a8ee5ae6310d1daa5483bac7eb887feeaa601

	Shader.set3flt(name + ".position", position);

	Shader.set3flt(name + ".attC", attC);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void mPointLight::render(shader Shader, int idx) {

	std::string name = "PointLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set3flt(name + ".attC", attC);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}

void uDirectLight::render(shader Shader) {

	std::string name = "DirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void mDirectLight::render(shader Shader) {

	std::string name = "DirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}

void uSpotLight::render(shader Shader, int idx) {

	std::string name = "SpotLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);
	Shader.set3flt(name + ".direction", direction);
	
	Shader.set_flt(name + ".innR", innR);
	Shader.set_flt(name + ".outR", outR);

	Shader.set3flt(name + ".attC", attC);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void mSpotLight::render(shader Shader, int idx) {

	std::string name = "SpotLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);
	Shader.set3flt(name + ".direction", direction);
	
	Shader.set_flt(name + ".innR", innR);
	Shader.set_flt(name + ".outR", outR);

	Shader.set3flt(name + ".attC", attC);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}
