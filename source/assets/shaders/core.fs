#version 330 core


//
//
// foos

// basic textured vec4
vec4 calculate__vec4PointLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculate__vec4DirectLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);
vec4 calculate__vec4SpotLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec);

// basic textured vec3
vec3 calculate__vec3TPointLight(int idx, vec3 normal, vec3 viewDir, vec3 tex);
vec3 calculate__vec3TDirectLight(vec3 normal, vec3 viewDir, vec3 tex);
vec3 calculate__vec3TSpotLight(int idx, vec3 normal, vec3 viewDir, vec3 tex);

// basic untextured vec3
vec3 calculate__vec3NPointLight(int idx, vec3 normal, vec3 viewDir);
vec3 calculate__vec3NDirectLight(vec3 normal, vec3 viewDir);
vec3 calculate__vec3NSpotLight(int idx, vec3 normal, vec3 viewDir);


//
//
// structs

// materials

struct vec3materialT { sampler2D diffuse; sampler2D specular; float reflectivity; };
struct vec3materialN { vec3 ambient; vec3 diffuse; vec3 specular; float reflectivity; };
struct vec4material { vec4 diffuse; vec4 specular; float reflectivity; };


//
// pointLights

// vec4
struct vec4pointLight { vec3 position; vec3 attC; vec4 ambient; vec4 diffuse; vec4 specular; };

#define max__vec4PointLights 10

uniform vec4pointLight vec4PointLights[max__vec4PointLights];
uniform int n__vec4PointLights;

// vec3
struct vec3pointLight { vec3 position; vec3 attC; vec3 ambient; vec3 diffuse; vec3 specular; };

#define max__vec3PointLights 10

uniform vec3pointLight vec3PointLights[max__vec3PointLights];
uniform int n__vec3PointLights;


//
// directLights

// vec4
struct vec4directLight { vec3 direction; vec4 ambient; vec4 diffuse; vec4 specular; };
uniform vec4directLight vec4DirectLight;

// vec3
struct vec3directLight { vec3 direction; vec3 ambient; vec3 diffuse; vec3 specular; };
uniform vec3directLight vec3DirectLight;


//
// spotLights

// vec4
struct vec4spotLight { vec3 position; vec3 direction; vec3 attC; float innR; float outR; vec4 ambient; vec4 diffuse; vec4 specular; };

#define max__vec4SpotLights 10

uniform vec4spotLight vec4SpotLights[max__vec4SpotLights];
uniform int n__vec4SpotLights;

// vec3
struct vec3spotLight { vec3 position; vec3 direction; vec3 attC; float innR; float outR; vec3 ambient; vec3 diffuse; vec3 specular; };

#define max__vec3SpotLights 10

uniform vec3spotLight vec3SpotLights[max__vec3SpotLights];
uniform int n__vec3SpotLights;



//
//
// other values

uniform int noTex;
uniform int nDims;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

out vec4 fragmentColor;

in vec3 vsOutFragPos;
in vec3 vsOutNormal;
in vec2 vsOutTexCoord;

uniform vec4material vec4Material;
uniform vec3materialN vec3MaterialN;
uniform vec3materialT vec3MaterialT;

uniform vec3 viewPos;



//
//
// here we go...

void main() {

	vec3 normal	= normalize(vsOutNormal);
	vec3 viewDir	= normalize(viewPos - vsOutFragPos);

	if (nDims == 4) {
		
		vec4 texDiff;
		vec4 texSpec;

		if (noTex == 1) {
		
			texDiff = vec4Material.diffuse;
			texSpec = vec4Material.specular;
		} else {
		
			texDiff = texture(diffuse0, vsOutTexCoord);
			texSpec = texture(specular0, vsOutTexCoord);
		}

		vec4 retval;

		// DirectLight
		retval = calculate__vec4DirectLight(normal, viewDir, texDiff, texSpec);

		// PointLight
		for (int i = 0; i < n__vec4PointLights; ++i) {

			retval += calculate__vec4PointLight(i, normal, viewDir, texDiff, texSpec);
		}

		//SpotLight	
		for (int i = 0; i < n__vec4SpotLights; ++i) {

			retval += calculate__vec4SpotLight(i, normal, viewDir, texDiff, texSpec);
		}

		fragmentColor = retval;
	} else {

		//	
	}
}

vec4 calculate__vec4PointLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec)  {

	//ambient PointLight
	vec4 ambient = vec4PointLights[idx].ambient * texDiff;


	//diffused PointLight
	vec3 pointLightDir = normalize(vec4PointLights[idx].position - vsOutFragPos);

	float diffComponent = max(dot(normal, pointLightDir), 0.0);

	vec4 diffuse = vec4PointLights[idx].diffuse * (diffComponent * texDiff); 


	//specular PointLight
	vec3 reflectDir = reflect(-pointLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), vec4Material.reflectivity * 128);

	vec4 specular = vec4PointLights[idx].specular * (specComponent * texDiff);


	//attenuation
	float distance = length(vec4PointLights[idx].position - vsOutFragPos);
	float attenuation = 1.0 / (vec4PointLights[idx].attC.x + vec4PointLights[idx].attC.y * distance + vec4PointLights[idx].attC.z * (distance * distance));

	return vec4(ambient + diffuse + specular) * attenuation;
}

vec4 calculate__vec4DirectLight(vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient DirectLight
	vec4 ambient = vec4DirectLight.ambient * texDiff;


	//diffused DirectLight
	vec3 directLightDir = normalize(-vec4DirectLight.direction);

	float diffComponent = max(dot(normal, directLightDir), 0.0);

	vec4 diffuse = vec4DirectLight.diffuse * (diffComponent * texDiff);


	//specular 
	vec3 reflectDir = reflect(-directLightDir, normal);

	float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), vec4Material.reflectivity * 128);

	vec4 specular = vec4DirectLight.specular * (specComponent * texSpec);


	return vec4(ambient + diffuse + specular);
}

vec4 calculateSpotLight(int idx, vec3 normal, vec3 viewDir, vec4 texDiff, vec4 texSpec) {

	//ambient SpotLight
	vec4 ambient = vec4SpotLights[idx].ambient * texDiff;
	
	vec3 spotLightDir = normalize(vec4SpotLights[idx].position - vsOutFragPos);

	float theta0 = dot(spotLightDir, normalize(-vec4SpotLights[idx].direction));

	if (theta0 > vec4SpotLights[idx].outR) {
		//if in inner Radius -> light it

		//diffused SpotLight
		float diffComponent = max(dot(normal, spotLightDir), 0.0);
		vec4 diffuse = vec4SpotLights[idx].diffuse * (diffComponent * texDiff);


		//specular SpotLight
		vec3 reflectDir = reflect(-spotLightDir, normal);

		float specComponent = pow(max(dot(viewDir, reflectDir), 0.0), vec4Material.reflectivity * 128);
		vec4 specular = vec4SpotLight[idx].specular * (specComponent * texSpec);


		//intensity
		float intensity = (theta0 - vec4SpotLights[idx].outR) / (vec4SpotLights[idx].innR - vec4SpotLights[idx].outR);
		intensity = clamp(intensity, 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;

		//attenuation
		float distance = length(vec4SpotLights[idx].position - vsOutFragPos);
		float attenuation = 1.0 / (vec4SpotLights[idx].attC.x + vec4SpotLights[idx].attC.y * distance + vec4SpotLights[idx].attC * (distance * distance));

		diffuse *= attenuation;
		specular *= attenuation;


		return vec4(ambient + diffuse + specular);
	}
	else {

		return ambient;
	}
}
