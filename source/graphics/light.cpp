#include "light.h"



//
//
// glm::vec3 !
void old_pointLight::render(shader Shader, int idx) {

	std::string name = "PointLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set_flt(name + ".k0", k0);
	Shader.set_flt(name + ".k1", k1);
	Shader.set_flt(name + ".k2", k2);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}

void old_directLight::render(shader Shader, int idx) {

	std::string name = "DirectLights[" + std::to_string(idx) + "]";
	//std::string name = "DirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set3flt(name + ".ambient", ambient);
	Shader.set3flt(name + ".diffuse", diffuse);
	Shader.set3flt(name + ".specular", specular);
}


void old_spotLight::render(shader Shader, int idx) {

	std::string name = "SpotLights[" + std::to_string(idx) + "]";
	//std::string name = "SpotLight";

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



//
//
// glm::vec4 !
void pointLight::render(shader Shader, int idx) {

	std::string name = "PointLights[" + std::to_string(idx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set_flt(name + ".k0", k0);
	Shader.set_flt(name + ".k1", k1);
	Shader.set_flt(name + ".k2", k2);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}

void directLight::render(shader Shader, int idx) {

	std::string name = "DirectLights[" + std::to_string(idx) + "]";
	//std::string name = "DirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}


void spotLight::render(shader Shader, int idx) {

	std::string name = "SpotLights[" + std::to_string(idx) + "]";
	//std::string name = "SpotLight";

	Shader.set3flt(name + ".position", position);
	Shader.set3flt(name + ".direction", direction);
	
	Shader.set_flt(name + ".cutOff", cutOff);
	Shader.set_flt(name + ".outerCutOff", outerCutOff);

	Shader.set_flt(name + ".k0", k0);
	Shader.set_flt(name + ".k1", k1);
	Shader.set_flt(name + ".k2", k2);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}
