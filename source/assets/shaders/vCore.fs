#version 330 core

//
//
// foos

vec3 calculatePointLight(int idx, vec3 normal, vec3 viewDir, vec3 tex);
vec3 calculateDirectLight(vec3 normal, vec3 viewDir, vec3 tex);
vec3 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec3 tex);



//
//
// materials

struct material { sampler2D diffuse; sampler2D specular; float reflectivity; };



//
//
// lights

// pointLights

struct pointLight { vec3 position; vec3 attC; vec3 ambient; vec3 diffuse; vec3 specular; };

#define maxPointLights 10

uniform pointLight PointLights[maxPointLights];
uniform int nPointLights;


// directLights

struct directLight { vec3 direction; vec3 ambient; vec3 diffuse; vec3 specular; };
uniform directLight DirectLight;


// spotLights

struct spotLight { vec3 position; vec3 direction; vec3 attC; float innR; float outR; vec3 ambient; vec3 diffuse; vec3 specular; };

#define maxSpotLights 10

uniform spotLight SpotLights[maxSpotLights];
uniform int nSpotLights;


// other vars

out vec4 fragmentColor;

in vec3 vsOutFragPos;
in vec3 vsOutNormal;
in vec2 vsOutTexCoord;

uniform material Material;

uniform vec3 viewPos;

void main() {

	vec3 normal	= normalize(vsOutNormal);
	vec3 viewDir	= normalize(viewPos - vsOutFragPos);

	// texture
	vec3 texture = vec3(texture(Material.diffuse, vsOutTexCoord));

	vec3 retval;

	// DirectLight

	retval = calculateDirectLight(normal, viewDir, texture);

	//PointLights

	for (int i = 0; i < nPointLights; ++i) {

		retval += calculatePointLight(i, normal, viewDir, texture);
	}

	//SpotLights

	for (int i = 0; i < nSpotLights; ++i) {

		retval += calculateSpotLight(i, normal, viewDir, texture);
	}

	fragmentColor = vec4(retval, 1.0); 
}

vec3 calculatePointLight(int IDx, vec3 normal, vec3 viewDir, vec3 tex)  {

	//ambient PointLight
	vec3 ambient = PointLights[IDx].ambient * tex;


	//diffused PointLight
	vec3 pointLightDir = normalize(PointLights[IDx].position - vsOutFragPos);

	float diffComponent = max(dot(normal, pointLightDir), 0.0);

	vec3 diffuse = PointLights[IDx].diffuse * (diffComponent * vec3(texture(Material.diffuse, vsOutTexCoord)));


	//specular PointLight
	vec3 reflectDir = reflect(-pointLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);

	vec3 specular = PointLights[IDx].specular * (specComponent * vec3(texture(Material.specular, vsOutTexCoord)));


	//attenuation
	float distance = length(PointLights[IDx].position - vsOutFragPos);
	float attenuation = 1.0 / (PointLights[IDx].k0 + PointLights[IDx].k1 * distance + PointLights[IDx].k2 * (distance * distance));

	return vec3(ambient + diffuse + specular) * attenuation;
}

vec3 calculateDirectLight(vec3 normal, vec3 viewDir, vec3 tex) {

	//ambient DirectLight
	vec3 ambient = DirectLight.ambient * tex;


	//diffused DirectLight
	vec3 directLightDir = normalize(-DirectLight.direction);

	float diffComponent = max(dot(normal, directLightDir), 0.0);

	vec3 diffuse = DirectLight.diffuse * (diffComponent * vec3(texture(Material.diffuse, vsOutTexCoord)));


	//specular 
	vec3 reflectDir = reflect(-directLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);

	vec3 specular = DirectLight.specular * (specComponent * vec3(texture(Material.specular, vsOutTexCoord)));


	return vec3(ambient + diffuse + specular);
}

vec3 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec3 tex) {

	//ambient SpotLight
	vec3 ambient = SpotLights[idx].ambient * tex;

	vec3 spotLightDir = normalize(SpotLights[idx].position - vsOutFragPos);

	float theta0 = dot(spotLightDir, normalize(-SpotLights[idx].direction));

	if (theta0 > SpotLights[idx].outR) {
		//if in cutOff -> light it

		//diffused SpotLight
		float diffComponent = max(dot(normal, spotLightDir), 0.0);
		vec3 diffuse = SpotLights[idx].diffuse * (diffComponent * vec3(texture(Material.diffuse, vsOutTexCoord)));


		//specular SpotLight
		vec3 reflectDir = reflect(-spotLightDir, normal);

		float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);
		vec3 specular = SpotLights[idx].specular * (specComponent * vec3(texture(Material.specular, vsOutTexCoord)));


		//intensity
		float intensity = (theta0 - SpotLights[idx].outR) / (SpotLights[idx].innR - SpotLights[idx].outR);
		intensity = clamp(intensity, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;


		//attenuation
		float distance = length(SpotLights[idx].position - vsOutFragPos);
		float attenuation = 1.0 / (SpotLights[idx].attC.x + SpotLights[idx].attC.y * distance + SpotLights[idx].attC.z * (distance * distance));

		diffuse *= attenuation;
		specular *= attenuation;


		return vec3(ambient + diffuse + specular);
	}
	else {

		return ambient;
	}
}
