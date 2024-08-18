#version 460 core

in vec3 fColour;
in vec2 fTexCoords;
in vec3 fNormals;
in vec3 fVertPos;
in vec3 fViewPos;

struct Material {
	sampler2D baseColourMap;
	sampler2D specularMap;
	float shininess;
	float specularStrength;
};

// the material for the shader to interface with our engine material
uniform Material material;

struct DirLight {
	vec3 colour;
	vec3 ambient;
	vec3 direction;
	float intensity;
};

struct PointLight {
	vec3 colour;
	vec3 position;
	float intensity;
	float linear;
	float quadratic;
};

#define NUM_DIR_LIGHTS 2
uniform DirLight dirLights[NUM_DIR_LIGHTS];

#define NUM_POINT_LIGHTS 20
uniform PointLight pointLights[NUM_POINT_LIGHTS];

out vec4 finalColour;

void main() {
	// final colour result for the vertex
	vec3 result = vec3(0.0f);

	// base colour that the object starts at
	vec3 baseColour = texture(material.baseColourMap, fTexCoords).rgb * fColour;

	// specular map value
	vec3 specularColour = texture(material.specularMap, fTexCoords).rgb;

	// get the view direction
	vec3 viewDir = normalize(fViewPos - fVertPos);


	//------------Directional Lights------------
	for (int i = 0; i < NUM_DIR_LIGHTS; i++) {
		// material light direction
		vec3 lightDir = normalize(-dirLights[i].direction);

		// get reflectioon light value
		vec3 reflectDir = reflect(-lightDir, fNormals);

		// how much light should show colour based on direction of normal facing the light
		float colourIntensity = max(dot(fNormals, lightDir), 0.0f);

		// ambient algorithm
		// minimum light value
		vec3 ambientLight = baseColour * dirLights[i].ambient;

		// light colour algorithm
		// basically just adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * dirLights[i].colour;
		lightColour *= colourIntensity;
		lightColour *= dirLights[i].intensity;

		// Specular power algorthim
		// calculates the shininess of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= dirLights[i].intensity;

		// add light values together to get the result
		result += (ambientLight + lightColour + specular);
	}

	//------------Point Lights------------
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		// light direction from the point light to the vertex
		vec3 lightDir = normalize(pointLights[i].position - fVertPos);

		// get reflectioon light value
		vec3 reflectDir = reflect(-lightDir, fNormals);

		// how much light should show colour based on direction of normal facing the light
		float diff = max(dot(fNormals, lightDir), 0.0f);

		//distance between the lights position and vertex position
		float distance = length(pointLights[i].position - fVertPos);

		// actual attenuation calculation
		float attenCalc = 1.0f + pointLights[i].linear * distance 
			+ pointLights[i].quadratic * (distance * distance);

		// distance that the light can reach
		// value between 1 and 0, 1 is full light, 0 is no light
		float attenuation = 0.0f;

		if(attenCalc != 0.0f){
			attenuation = 1.0f / attenCalc;
		}

		// light colour algorithm
		// basically just adjusts how much colour you can see based on the normal direction
		vec3 lightColour = baseColour * pointLights[i].colour;
		lightColour *= diff;
		lightColour *= attenuation;
		lightColour *= pointLights[i].intensity;

		// Specular power algorthim
		// calculates the shininess of the model
		float specPower = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = specularColour * specPower;
		specular *= material.specularStrength;
		specular *= pointLights[i].intensity;

		// add light values together to get the result
		result += (lightColour + specular);
	}

	finalColour = vec4(result, 1.0f);
}