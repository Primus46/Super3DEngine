#pragma once
#include "Math/SSTTransform.h"
#include "Game/SGameEngine.h"

struct SSTCamera {
	SSTCamera() {
		fov = 70.0f;
		defaultFov = fov;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		moveSpeed = 50.0f;
		rotateSpeed = 0.005f;/* my mouse dpi is set to 3800, 
		if your dpi is lower increase this value for faster rotation or decrease it if your mouse
		dpi is higher */
	}

	// rotate the camera based on the rotation passed in
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f)) {

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

		glm::vec3 direction = MoveDir * scale;

		float deltaTime = 1.0f;

		if (const auto & ge = SGameEngine::GetGameEngine())
		{
			deltaTime = ge->DeltaTimef();
		}

		transform.position += direction * moveSpeed * deltaTime;
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
