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

		TShared<SSTMaterial> mat3 = TMakeShared<SSTMaterial>();
		TShared<SSTMaterial> mat4 = TMakeShared<SSTMaterial>();

		mat3->m_baseColourMap = GemtTex1;
		mat3->m_specularMap = GemSpecTex1;

		mat4->m_baseColourMap = GemtTex2;
		mat4->m_specularMap = GemSpecTex2;

		modelRef->SetMaterialBySlot(0, mat3);
		modelRef->SetMaterialBySlot(1, mat4);
	}

	AddCollision({ GetTransform().position, glm::vec3(10.0f) });
}

void Gem::OnTick(float deltaTime)
{
	SWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 10.0f * deltaTime;
}

void Gem::OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& otherCol)
{
	if (otherCol->type == SECollisionTypes::PLAYER)
	{
		Destroy();
	}
}
