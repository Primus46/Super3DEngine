#include "Game/GameObjects/MyObjects/Chest.h"
#include "Graphics/SModel.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTMaterial.h"

Chest::Chest()
{
}

void Chest::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Chest/RenderAnim.fbx").lock())
	{
		TShared<STexture> ChestTex = TMakeShared<STexture>();
		ChestTex->LoadTexture("Chest base colour", "Models/Chest/Textures/Chest_BaseColour.tga.png");

		TShared<STexture> ChestSpecTex = TMakeShared<STexture>();
		ChestSpecTex->LoadTexture("Chest spec colour", "Models/Chest/Textures/Chest_Specular.tga.png");

		TShared<STexture> ChestLockTex = TMakeShared<STexture>();
		ChestLockTex->LoadTexture("Chest lock base colour", "Models/Chest/Textures/Head_BaseColour.tga.png");

		TShared<STexture> ChestLockSpecTex = TMakeShared<STexture>();
		ChestLockSpecTex->LoadTexture("Chest lock spec colour", "Models/Chest/Textures/Head_Roughness.tga.png");

		TShared<SSTMaterial> mat = TMakeShared<SSTMaterial>();
		TShared<SSTMaterial> mat2 = TMakeShared<SSTMaterial>();

		mat->m_baseColourMap = ChestTex;
		mat->m_specularMap = ChestSpecTex;

		mat2->m_baseColourMap = ChestLockTex;
		mat2->m_specularMap = ChestLockSpecTex;

		modelRef->SetMaterialBySlot(0, mat);
		modelRef->SetMaterialBySlot(1, mat2);
	}

	AddCollision({ GetTransform().position, glm::vec3(20.0f, 20.0f, 20.0f) });
}

void Chest::OnTick(float deltaTime)
{
		SWorldObject::OnTick(deltaTime);

}

void Chest::OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& col, const TShared<SSTCollision>& otherCol)
{
	if (otherCol->type == SECollisionTypes::PLAYER)
	{
		SDebug::Log("You got the chest You Win!");
		col->debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		//Destroy();
	}
}
