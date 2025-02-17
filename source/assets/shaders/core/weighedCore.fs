#version 330 core

vec4 calculatePointLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculateDirectLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);

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

#define maxDirectLights 3
struct directLight {

	vec3 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform directLight DirectLights[maxDirectLights];
uniform int nDirectLights;

#define maxSpotLights 10
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
uniform spotLight SpotLights[maxSpotLights];
uniform int nSpotLights;

in vec3 vsOutFragPos;
in vec3 vsOutNormal;
in vec3 vsOutColor;
in vec2 vsOutTextureCoords;
in vec4 vsOutWVBones;
in vec4 vsOutWeights;

uniform int noTex;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec3 viewPos;

uniform float nBone;

uniform material Material;

out vec4 fragmentColor;

void main() {

	vec3 normal	= normalize(vsOutNormal);
	vec3 viewDir	= normalize(viewPos - vsOutFragPos);

	vec4 texDiff;
	vec4 texSpec;

	if (noTex == 1) {
		
		texDiff = Material.diffuse;
		texSpec = Material.specular;
	} else {
		
		texDiff = texture(diffuse0, vsOutTextureCoords);
		texSpec = texture(specular0, vsOutTextureCoords);
	}
	
	vec4 retval;

	//DirectLight
	for (int i = 0; i < nDirectLights; ++i) {

		retval += calculateDirectLight(i, normal, viewDir, texDiff, texSpec);
	}

	//PointLight
	for (int i = 0; i < nPointLights; ++i) {

		retval += calculatePointLight(i, normal, viewDir, texDiff, texSpec);
	}

	//SpotLight
	for (int i = 0; i < nSpotLights; ++i) {

		retval += calculateSpotLight(i, normal, viewDir, texDiff, texSpec);
	}
	
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


vec4 calculateDirectLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient DirectLight
	vec4 ambient = DirectLights[idx].ambient * texDiff;


	//diffused DirectLight
	vec3 directLightDir = normalize(-DirectLights[idx].direction);

	float diffComponent = max(dot(normal, directLightDir), 0.0);

	vec4 diffuse = DirectLights[idx].diffuse * (diffComponent * texDiff);


	//specular 
	vec3 reflectDir = reflect(-directLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);

	vec4 specular = DirectLights[idx].specular * (specComponent * texSpec);


	return vec4(ambient + diffuse + specular);
}


vec4 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient SpotLight
	vec4 ambient = SpotLights[idx].ambient * texDiff;

	vec3 spotLightDir = normalize(SpotLights[idx].position - vsOutFragPos);

	float theta0 = dot(spotLightDir, normalize(-SpotLights[idx].direction));

	if (theta0 > SpotLights[idx].outerCutOff) {
		//if in cutOff -> light it

		//diffused SpotLight
		float diffComponent = max(dot(normal, spotLightDir), 0.0);
		vec4 diffuse = SpotLights[idx].diffuse * (diffComponent * texDiff);


		//specular SpotLight
		vec3 reflectDir = reflect(-spotLightDir, normal);

		float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);
		vec4 specular = SpotLights[idx].specular * (specComponent * texSpec);


		//intensity
		float intensity = (theta0 - SpotLights[idx].outerCutOff) / (SpotLights[idx].cutOff - SpotLights[idx].outerCutOff);
		intensity = clamp(intensity, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;


		//attenuation
		float distance = length(SpotLights[idx].position - vsOutFragPos);
		float attenuation = 1.0 / (SpotLights[idx].k0 + SpotLights[idx].k1 * distance + SpotLights[idx].k2 * (distance * distance));

		diffuse *= attenuation;
		specular *= attenuation;


		return vec4(ambient + diffuse + specular);
	}
	else {

		return ambient;
	}
}
