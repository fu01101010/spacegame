#include "light.h"


// pointLight class
template< typename glm__vecN > 
pointLight< glm__vecN >::pointLight(glm::vec3 position, glm::vec3 attC, glm__vecN ambient, glm__vecN diffuse, glm__vecN specular) 
		: position (position), attC (attC), ambient (ambient), diffuse (diffuse), specular (specular) { }

template< typename glm__vecN > 
void pointLight< glm__vecN >::render(shader Shader, int idx) {
	
	if (typeid(glm__vecN).name() == typeid(glm::vec3).name()) {
		
		// 3dimensional light case
		std::string name =  "vec3PointLights[" + std::to_string(idx) + "]";

		Shader.set3flt(name + ".position", position);

		Shader.set_flt(name + ".attC.x", attC.x);
		Shader.set_flt(name + ".attC.y", attC.y);
		Shader.set_flt(name + ".attC.z", attC.z);

		Shader.set3flt(name + ".ambient", ambient);
		Shader.set3flt(name + ".diffuse", diffuse);
		Shader.set3flt(name + ".specular", specular);
	} else if (typeid(glm__vecN).name() == typeid(glm::vec4).name()) {
			
		// 4dimensional light case
		std::string name = "vec4PointLights[" + std::to_string(idx) + "]";

		Shader.set3flt(name + ".position", position);

		Shader.set_flt(name + ".attC.x", attC.x);
		Shader.set_flt(name + ".attC.y", attC.y);
		Shader.set_flt(name + ".attC.z", attC.z);

		Shader.set4flt(name + ".ambient", ambient);
		Shader.set4flt(name + ".diffuse", diffuse);
		Shader.set4flt(name + ".specular", specular);
	}	
}


// directLight class
template< typename glm__vecN > 
directLight< glm__vecN >::directLight(glm::vec3 direction, glm__vecN ambient, glm__vecN diffuse, glm__vecN specular) 
		: direction (direction), ambient (ambient), diffuse (diffuse), specular (specular) { }

template< typename glm__vecN > 
void directLight< glm__vecN >::render(shader Shader) {

	if (typeid(glm__vecN).name() == typeid(glm::vec3).name()) {
		
		// 3dimensional light case
		std::string name =  "vec3DirectLight";

		Shader.set3flt(name + ".direction", direction);

		Shader.set3flt(name + ".ambient", ambient);
		Shader.set3flt(name + ".diffuse", diffuse);
		Shader.set3flt(name + ".specular", specular);
	} else if (typeid(glm__vecN).name() == typeid(glm::vec4).name()) {
			
		// 4dimensional light case
		std::string name =  "vec4DirectLight";

		Shader.set3flt(name + ".direction", direction);

		Shader.set4flt(name + ".ambient", ambient);
		Shader.set4flt(name + ".diffuse", diffuse);
		Shader.set4flt(name + ".specular", specular);
	}	
}


// spotLight class
template< typename glm__vecN > 
spotLight< glm__vecN >::spotLight(glm::vec3 position, glm::vec3 direction, float innR, float outR, glm::vec3 attC, glm__vecN ambient, glm__vecN diffuse, glm__vecN specular) 
		: position (position), direction (direction), innR (innR), outR (outR), attC (attC), ambient (ambient), diffuse (diffuse), specular (specular) { }

template< typename glm__vecN > 
void spotLight< glm__vecN >::render(shader Shader, int idx) {

	if (typeid(glm__vecN).name() == typeid(glm::vec3).name()) {
		
		// 3dimensional light case
		std::string name =  "vec3SpotLights[" + std::to_string(idx) + "]";

		Shader.set3flt(name + ".position", position);
		Shader.set3flt(name + ".direction", direction);
	
		Shader.set_flt(name + ".innR", innR);
		Shader.set_flt(name + ".outR", outR);

		Shader.set_flt(name + ".attC.x", attC.x);
		Shader.set_flt(name + ".attC.y", attC.y);
		Shader.set_flt(name + ".attC.z", attC.z);

		Shader.set3flt(name + ".ambient", ambient);
		Shader.set3flt(name + ".diffuse", diffuse);
		Shader.set3flt(name + ".specular", specular);
	} else if (typeid(glm__vecN).name() == typeid(glm::vec4).name()) {
			
		// 4dimensional light case
		std::string name =  "vec4SpotLights[" + std::to_string(idx) + "]";

		Shader.set3flt(name + ".position", position);
		Shader.set3flt(name + ".direction", direction);
	
		Shader.set_flt(name + ".innR", innR);
		Shader.set_flt(name + ".outR", outR);

		Shader.set_flt(name + ".attC.x", attC.x);
		Shader.set_flt(name + ".attC.y", attC.y);
		Shader.set_flt(name + ".attC.z", attC.z);

		Shader.set4flt(name + ".ambient", ambient);
		Shader.set4flt(name + ".diffuse", diffuse);
		Shader.set4flt(name + ".specular", specular);
	}	
}
