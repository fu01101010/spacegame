#version 330 core

//
//
// foos

// only textured items for now

vec4 calculatePointLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculateDirectLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);



//
//
// materials

struct material { vec4 diffuse; vec4 specular; float reflectivity; };



//
//
// lights

// pointLights

struct pointLight { vec3 position; vec3 attC; vec4 ambient; vec4 diffuse; vec4 specular; };

#define maxPointLights 10

uniform pointLight PointLights[maxPointLights];
uniform int nPointLights;


// directLights

struct directLight { vec3 direction; vec4 ambient; vec4 diffuse; vec4 specular; };
uniform directLight DirectLight;


// spotLights

struct spotLight { vec3 position; vec3 direction; float innR; float outR; vec3 attC; vec4 ambient; vec4 diffuse; vec4 specular; };

#define maxSpotLights 10

uniform spotLight SpotLights[maxSpotLights];
uniform int nSpotLights;



//
//
// other vars

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

	// DirectLight

	retval = calculateDirectLight(normal, viewDir, texDiff, texSpec);

	// PointLights

	for (int i = 0; i < nPointLights; ++i) {

		retval += calculatePointLight(i, normal, viewDir, texDiff, texSpec);
	}

	// SpotLights

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
	float attenuation = 1.0 / (PointLights[idx].attC.x + PointLights[idx].attC.y * distance + PointLights[idx].attC.z * (distance * distance));

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

vec4 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient SpotLight
	vec4 ambient = SpotLights[idx].ambient * texDiff;

	vec3 spotLightDir = normalize(SpotLights[idx].position - vsOutFragPos);

	float theta0 = dot(spotLightDir, normalize(-SpotLights[idx].direction));

	if (theta0 > SpotLights[idx].outR) {
		//if in innR -> light it

		//diffused SpotLight
		float diffComponent = max(dot(normal, spotLightDir), 0.0);
		vec4 diffuse = SpotLights[idx].diffuse * (diffComponent * texDiff);


		//specular SpotLight
		vec3 reflectDir = reflect(-spotLightDir, normal);

		float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity * 128);
		vec4 specular = SpotLights[idx].specular * (specComponent * texSpec);


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


		return vec4(ambient + diffuse + specular);
	} else {

		return ambient;
	}
}
