#pragma once

// External Libs
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

struct SSTTransform {
	SSTTransform() {
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
	}

	// get the forward vector of the local rotation
	glm::vec3 Forward() {
		glm::vec3 forward = glm::vec3(0.0f);

		// get the forward x value by * sin of y by the cos od x
		forward.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		// get the forward y value from the sin of x
		forward.y = sin(glm::radians(rotation.x));
		// forward z value by * cos of y and the cos of x
		forward.z = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

		// make sure we don't normalize 0
		if(glm::length(forward) != 0.0f) {
			glm::normalize(forward);
		}

		return forward;
	}

	// get the right vector of the local rotation
	glm::vec3 Right() {
		// get right value by crossing the forward and world up vector
		glm::vec3 right = glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f));

		// make sure we don't normalize 0
		if (glm::length(right) != 0.0f) {
			glm::normalize(right);
		}

		return right;
	}

	// get the up vector of the local rotation
	glm::vec3 Up() {
		// get the up vector by crossing the right and forward vector
		glm::vec3 up = glm::cross(Right(), Forward());

		// make sure we don't normalize 0
		if (glm::length(up) != 0.0f) {
			glm::normalize(up);
		}

		return up;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};