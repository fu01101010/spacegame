#version 330 core

vec4 calculatePointLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculateDirectLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculateSpotLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);

struct material {

	vec4 diffuse;
	vec4 specular;

	float reflectivity;
};

#define maxPointLights 10
struct pointLight {

	vec3 position;

	float k0;
	float k1;
	float k2;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform pointLight PointLights[maxPointLights];
uniform int nPointLights;

struct directLight {

	vec3 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform directLight DirectLight;

struct spotLight {

	vec3 position;
	vec3 direction;

	float k0;
	float k1;
	float k2;

	float cutOff;
	float outerCutOff;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform spotLight SpotLight;

uniform int noTex;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

out vec4 fragmentColor;

in vec3 vsOutFragPos;
in vec3 vsOutNormal;
in vec2 vsOutTexCoord;

uniform material Material;

uniform vec3 viewPos;

void main() {

	vec3 normal	= normalize(vsOutNormal);
	vec3 viewDir	= normalize(viewPos - vsOutFragPos);

	vec4 texDiff;
	vec4 texSpec;

	if (noTex == 1) {
		
		texDiff = Material.diffuse;
		texSpec = Material.specular;
	} else {
		
		texDiff = texture(diffuse0, vsOutTexCoord);
		texSpec = texture(specular0, vsOutTexCoord);
	}

	vec4 retval;

	//DirectLight
	retval = calculateDirectLight(normal, viewDir, texDiff, texSpec);

	//PointLight
	for (int i = 0; i < nPointLights; ++i) {

		retval += calculatePointLight(i, normal, viewDir, texDiff, texSpec);
	}

	//SpotLight	
	retval += calculateSpotLight(normal, viewDir, texDiff, texSpec);

	fragmentColor = retval;
}

vec4 calculatePointLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec)  {

	//ambient PointLight
	vec4 ambient = PointLights[idx].ambient * texDiff;


	//diffused PointLight
	vec3 pointLightDir = normalize(PointLights[idx].position - vsOutFragPos);

	float diffComponent = max(dot(normal, pointLightDir), 0.0);

	vec4 diffuse = PointLights[idx].diffuse * (diffComponent * texDiff); 


	//specular PointLight
	vec3 reflectDir = reflect(-pointLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);

	vec4 specular = PointLights[idx].specular * (specComponent * texDiff);


	//attenuation
	float distance = length(PointLights[idx].position - vsOutFragPos);
	float attenuation = 1.0 / (PointLights[idx].k0 + PointLights[idx].k1 * distance + PointLights[idx].k2 * (distance * distance));

	return vec4(ambient + diffuse + specular) * attenuation;
}

vec4 calculateDirectLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient DirectLight
	vec4 ambient = DirectLight.ambient * texDiff;


	//diffused DirectLight
	vec3 directLightDir = normalize(-DirectLight.direction);

	float diffComponent = max(dot(normal, directLightDir), 0.0);

	vec4 diffuse = DirectLight.diffuse * (diffComponent * texDiff);


	//specular 
	vec3 reflectDir = reflect(-directLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);

	vec4 specular = DirectLight.specular * (specComponent * texSpec);


	return vec4(ambient + diffuse + specular);
}

vec4 calculateSpotLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient SpotLight
	vec4 ambient = SpotLight.ambient * texDiff;

	vec3 spotLightDir = normalize(SpotLight.position - vsOutFragPos);

	float theta0 = dot(spotLightDir, normalize(-SpotLight.direction));

	if (theta0 > SpotLight.outerCutOff) {
		//if in cutOff -> light it

		//diffused SpotLight
		float diffComponent = max(dot(normal, spotLightDir), 0.0);
		vec4 diffuse = SpotLight.diffuse * (diffComponent * texDiff);


		//specular SpotLight
		vec3 reflectDir = reflect(-spotLightDir, normal);

		float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);
		vec4 specular = SpotLight.specular * (specComponent * texSpec);


		//intensity
		float intensity = (theta0 - SpotLight.outerCutOff) / (SpotLight.cutOff - SpotLight.outerCutOff);
		intensity = clamp(intensity, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;


		//attenuation
		float distance = length(SpotLight.position - vsOutFragPos);
		float attenuation = 1.0 / (SpotLight.k0 + SpotLight.k1 * distance + SpotLight.k2 * (distance * distance));

		diffuse *= attenuation;
		specular *= attenuation;


		return vec4(ambient + diffuse + specular);
	}
	else {

		return ambient;
	}
}
