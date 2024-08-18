#pragma once
#include "Math/SSTBox.h"
#include "EngineTypes.h"

enum class SECollisionTypes {
	ALL,
	COLLECTIBLE,
	PLAYER
};

class SMesh;

struct SSTCollision {
	SSTCollision() {
		box.position = glm::vec3(0.0f);
		box.halfSize = glm::vec3(0.0f);
		type = SECollisionTypes::ALL;
		debugColour = glm::vec3(1.0f, 0.0f, 0.0f); // r g b
		tag = "";
	}

	// determine if 2 collisions are overlapping
	static bool IsOverLapping(const SSTCollision& col1, const SSTCollision& col2) {
		return SSTBox::BoxOverlap(col1.box, col2.box);
	}

	// store the debug mesh
	TShared<SMesh> debugMesh;

	// colour of the debug wireframe
	glm::vec3 debugColour;

	// collision types
	SECollisionTypes type;

	// custom tag for detection
	SString tag;

	// box bounds
	SSTBox box;
};