#include "Game/GameObjects/MyObjects/Coin.h"
#include "Graphics/SModel.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTMaterial.h"

Coin::Coin()
{
}

void Coin::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Coin/Coin_LP1.fbx").lock())
	{
		TShared<STexture> CoinTex = TMakeShared<STexture>();
		CoinTex->LoadTexture("Coin base colour", "Models/Coin/textures/Coin_LP1_Gold_BaseColor.png");

		TShared<SSTMaterial> mat = TMakeShared<SSTMaterial>();
		mat->m_baseColourMap = CoinTex;
		mat->specularStrength = 0.1f;

		modelRef->SetMaterialBySlot(0, mat);
	}

	AddCollision({ GetTransform().position, glm::vec3(10.0f, 15.0f, 10.0f) });

	
}

void Coin::OnTick(float deltaTime)
{
	SWorldObject::OnTick(deltaTime);

	GetTransform().rotation.z -= 100.0f * deltaTime;
}

void Coin::OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& col, const TShared<SSTCollision>& otherCol)
{
	if (otherCol->type == SECollisionTypes::PLAYER)
	{
		SDebug::Log("Coin Overlapped with Player");
		col->debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		Destroy();
	}
}
