#include "dLight.h"

void dPointLight::render(shader Shader, int IDx) {

	std::string name = "PointLights[" + std::to_string(IDx) + "]";

	Shader.set3flt(name + ".position", position);

	Shader.set_flt(name + ".k0", k0);
	Shader.set_flt(name + ".k1", k1);
	Shader.set_flt(name + ".k2", k2);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}

void dDirectLight::render(shader Shader) {

	std::string name = "DirectLight";

	Shader.set3flt(name + ".direction", direction);

	Shader.set4flt(name + ".ambient", ambient);
	Shader.set4flt(name + ".diffuse", diffuse);
	Shader.set4flt(name + ".specular", specular);
}


void dSpotLight::render(shader Shader) {

	//std::string name = "SpotLights[" + std::to_string(IDx) + "]";
	std::string name = "SpotLight";

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
