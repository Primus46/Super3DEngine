#pragma once

// External Libs
#include <GLM/vec3.hpp>

struct SSTLight {
	SSTLight() {
		colour = glm::vec3(1.0f);
		intensity = 1.0f;
	}
	virtual ~SSTLight() = default;

	glm::vec3 colour;
	float intensity;
};

struct SSTDirLight : public SSTLight {
	SSTDirLight() {
		ambient = glm::vec3(0.01f);
		direction = glm::vec3(0.0f);
	}

	glm::vec3 ambient;
	glm::vec3 direction;
};

struct SSTPointLight : public SSTLight {
	SSTPointLight() {
		position = glm::vec3(0.0f);
		linear = 0.1f;
		quadratic = 0.035f;
	}

	glm::vec3 position;

	// fall off values for how far the light can reach
	float linear;
	float quadratic;
};