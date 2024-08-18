#pragma once
#include <GLM/vec3.hpp>
#include <GLM/geometric.hpp>

struct SSTBox {
	SSTBox() {
		position = glm::vec3(0.0f);
		halfSize = glm::vec3(0.0f);
	}

	SSTBox(glm::vec3 position, glm::vec3 halfSize) 
		: position(position), halfSize(halfSize) {}

	// check if 2 boxes are overlapping
	static bool BoxOverlap(const SSTBox& box1, const SSTBox& box2) {
		// ensures that a 0 size always returns false
		if (glm::length(box1.halfSize) == 0.0f || glm::length(box2.halfSize) == 0.0f) {
			return false;
		}

		// detect if x is in bounds
		if (box1.GetMin().x > box2.GetMax().x || box1.GetMax().x < box2.GetMin().x) {
			return false;
		}

		// detect if y is in bounds
		if (box1.GetMin().y > box2.GetMax().y || box1.GetMax().y < box2.GetMin().y) {
			return false;
		}

		// detect if z is in bounds
		if (box1.GetMin().z > box2.GetMax().z || box1.GetMax().z < box2.GetMin().z) {
			return false;
		}

		return true;
	}

	// largest vector value position of the box
	glm::vec3 GetMax() const {
		return position + halfSize;
	}

	// smallest vector value position of the box
	glm::vec3 GetMin() const {
		return position - halfSize;
	}

	// the boxes position in the world
	glm::vec3 position;

	// size of the box, will start from the center so half size
	glm::vec3 halfSize;
};
