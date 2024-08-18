#pragma once
#include "Game/GameObjects/SObject.h"
#include "Math/SSTTransform.h"
#include "Math/SSTCollision.h"

class SModel;

class SWorldObject : public SObject {
public:
	SWorldObject() = default;
	virtual ~SWorldObject() = default;

	// inport a model and return a weak reference
	TWeak<SModel> ImportModel(const SString& path);

	// add a collision to the object
	TWeak<SSTCollision> AddCollision(const SSTBox& box, const bool& debud = false);

	SSTTransform& GetTransform() { return m_transform; }

	// run a test to see if another object is overlapping
	void TestCollision(const TShared<SWorldObject>& other);

	// does the object have collisions
	bool HasCollisions() const { return m_objectCollisions.size() > 0; }

protected:
	void OnPostTick(float deltaTime) override;

	// on collision overlap
	// detect every frame a collision is overlapped
	virtual void OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& otherCol) {}

private:
	// transform in world space
	SSTTransform m_transform;

	// store any models attached to this object
	TArray<TShared<SModel>> m_objectModels;

	// store  the collisions for the model
	TArray<TShared<SSTCollision>> m_objectCollisions;
};