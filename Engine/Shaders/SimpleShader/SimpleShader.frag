#version 460 core

in vec3 fColour;
in vec2 fTexCoords;
in vec3 fNormals;

uniform sampler2D colourMap;

struct DirLight {
	vec3 colour;
	vec3 ambient;
	vec3 direction;
	float intensity;
};

#define NUM_DIR_LIGHTS 2
uniform DirLight dirLights[NUM_DIR_LIGHTS];

out vec4 finalColour;

void main() {
	// final colour result for the vertex
	vec4 result = vec4(vec3(0.0f), 1.0f);

	// base colour that the object starts at
	vec4 baseColour = texture(colourMap, fTexCoords) * vec4(fColour, 1.0);

	for (int i = 0; i < NUM_DIR_LIGHTS; i++) {
		// material light direction
		vec3 lightDir = normalize(-dirLights[i].direction);

		// get reflectioon light value
		vec3 reflectDir = reflect(-lightDir, fNormals);

		// how much light should show colour based on direction of normal facing the light
		float colourIntensity = max(dot(fNormals, lightDir), 0.0f);

		// ambient algorithm
		// minimum light value
		vec4 ambientLight = vec4(max(dirLights[i].ambient, baseColour.rgb), 1.0f);

		// light colour algorithm
		// basically just adjusts how much colour you can see based on the normal direction
		vec4 lightColour = vec4(dirLights[i].colour, 1.0f);
		lightColour *= colourIntensity;
		lightColour *= dirLights[i].intensity;

		// add light values together to get the result
		result += ambientLight + lightColour;
	}

	finalColour = result;
}