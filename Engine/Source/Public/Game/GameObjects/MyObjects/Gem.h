#pragma once
#include "Game/GameObjects/SWorldObject.h"

class Gem : public SWorldObject {
public:
	Gem();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<SWorldObject>& other,
		const TShared<SSTCollision>& col, const TShared<SSTCollision>& otherCol) override;

};