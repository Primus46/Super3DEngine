#include "Game/GameObjects/MyObjects/Gem.h"
#include "Graphics/SModel.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTMaterial.h"

Gem::Gem()
{

}

void Gem::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Gem/gem1.glb").lock()) 
	{
		TShared<STexture> GemtTex1 = TMakeShared<STexture>();
		GemtTex1->LoadTexture("Gem base colour", "Models/Gem/Textures/gem1_EMISSIVE_0.jpeg");

		TShared<STexture> GemSpecTex1 = TMakeShared<STexture>();
		GemSpecTex1->LoadTexture("Gem spec colour", "Models/Gem/Textures/gem1_NORMAL_1.jpeg");

		TShared<STexture> GemtTex2 = TMakeShared<STexture>();
		GemtTex2->LoadTexture("Gem base colour", "Models/Gem/Textures/gem1_BASECOLOR_2.jpeg");

		TShared<STexture> GemSpecTex2 = TMakeShared<STexture>();
		GemSpecTex2->LoadTexture("Gem spec colour", "Models/Gem/Textures/gem1_Specular.png");

		TShared<SSTMaterial> mat = TMakeShared<SSTMaterial>();
		TShared<SSTMaterial> mat2 = TMakeShared<SSTMaterial>();

		mat->m_baseColourMap = GemtTex1;
		mat->m_specularMap = GemSpecTex1;
		mat->specularStrength = 0.1f;

		mat2->m_baseColourMap = GemtTex2;
		mat2->m_specularMap = GemSpecTex2;
		mat2->specularStrength = 0.1f;

		modelRef->SetMaterialBySlot(0, mat);
		modelRef->SetMaterialBySlot(1, mat2);
	}

	AddCollision({ GetTransform().position, glm::vec3(5.0f, 7.0f, 5.0f) });
}

void Gem::OnTick(float deltaTime)
{
	SWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 100.0f * deltaTime;
}

void Gem::OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& col, const TShared<SSTCollision>& otherCol)
{
	if (otherCol->type == SECollisionTypes::PLAYER)
	{
		SDebug::Log("Gem Overlap with Player");
		col->debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		Destroy();
	}
}
