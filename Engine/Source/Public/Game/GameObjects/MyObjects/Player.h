#pragma once
#include "Game/GameObjects/SWorldObject.h"

class Player : public SWorldObject {
public:
	Player();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

};