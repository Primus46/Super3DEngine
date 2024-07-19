#pragma once
#include "Math/SSTTransform.h"

struct SSTCamera {
	SSTCamera() {
		fov = 70.0f;
		defaultFov = fov;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		moveSpeed = 0.001f;
		rotateSpeed = 0.1f;
	}

	// rotate the camera based on the rotation passed in
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f)) {
		if (glm::length(rotation) != 0.0f)
			rotation = glm::normalize(rotation);

		transform.rotation += rotation * scale * rotateSpeed;

		if (transform.rotation.x < -89.0f) {
			transform.rotation.x = -89.0f;
		}

		if (transform.rotation.x > 89.0f) {
			transform.rotation.x = 89.0f;
		}
	}

	// translate the camera based on the translation passed in
	void Translate(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f)) {
		// move the input direction forward if required
		glm::vec3 MoveDir = transform.Forward() * translation.z;
		MoveDir += transform.Right() * translation.x;
		MoveDir.y += translation.y;

		if (glm::length(MoveDir) != 0.0f)
			MoveDir = glm::normalize(MoveDir);

		transform.position += MoveDir * scale * moveSpeed;
	}

	// zoom in the camera based on the amount added
	void Zoom(const float& amount) {
		fov -= amount;

		if(fov < 1.0f) {
			fov = 1.0f;
		}

		if (fov > 179.0f) {
			fov = 179.0f;
		}

		SDebug::Log("Zoom: " + std::to_string(fov));
	}

	void ResetZoom() {
		fov = defaultFov;
	}

	// use to set the fov so that the default fov is updated
	void SetFOV(const float& newFov) {
		fov = newFov;
		defaultFov = defaultFov;
	}
	
	SSTTransform transform;
	float fov;
	// donn't change will auto set based on the fov on initialise
	float defaultFov;
	float aspectRatio;
	float nearClip;
	float farClip;
	float moveSpeed;
	float rotateSpeed;
};
