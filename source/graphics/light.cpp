#include "light.h"

void uPointLight::render(shader Shader, int idx) {

	std::string name = "PointLights[" + std::to_string(idx) + "]";

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
