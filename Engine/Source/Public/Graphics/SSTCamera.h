#pragma once
#include "Math/SSTTransform.h"

struct SSTCamera {
	SSTCamera() {
		fov = 70.0f;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
	}

	
	SSTTransform transform;
	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;
};
