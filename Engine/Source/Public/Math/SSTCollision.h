#pragma once
#include "Math/SSTBox.h"
#include "EngineTypes.h"

enum class SECollisionTypes {
	ALL,
	COLLECTIBLE,
	PLAYER
};

struct SSTCollision {
	SSTCollision() {
		box.position = glm::vec3(0.0f);
		box.halfSize = glm::vec3(0.0f);
		type = SECollisionTypes::ALL;
		tag = "";
	}

	// determine if 2 collisions are overlapping
	static bool IsOverLapping(const SSTCollision& col1, const SSTCollision& col2) {
		return SSTBox::BoxOverlap(col1.box, col2.box);
	}

	// collision types
	SECollisionTypes type;

	// custom tag for detection
	SString tag;

	// box bounds
	SSTBox box;
};