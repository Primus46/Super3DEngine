#include "Game/GameObjects/MyObjects/Player.h"
#include "Graphics/SGraphicsEngine.h"
#include "Graphics/SSTCamera.h"


Player::Player()
{
	
}

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(10.0f) }, true).lock())
	{
		colRef->type = SECollisionTypes::PLAYER;
	}
}

void Player::OnTick(float deltaTime)
{
	SWorldObject::OnTick(deltaTime);

	if (const auto& camRef = SGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock())
	{
		GetTransform() = camRef->transform;
	}

	//SDebug::Log(std::to_string(GetTransform().position.x));
}