#pragma once
#include "Game/GameObjects/SWorldObject.h"

class Helmet : public SWorldObject {
public:
	Helmet();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& otherCol) override;
};