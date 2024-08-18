#include "Game/GameObjects/MyObjects/Star.h"
#include "Graphics/SModel.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTMaterial.h"

Star::Star()
{
}

void Star::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Star/gold star.fbx").lock()) 
	{
		TShared<STexture> starTex = TMakeShared<STexture>();
		starTex->LoadTexture("Star base colour", "Models/Star/Textures/gold star_Star_BaseColor.png");

		TShared<SSTMaterial> mat = TMakeShared<SSTMaterial>();

		mat->m_baseColourMap = starTex;
		mat->specularStrength = 0.1f;

		modelRef->SetMaterialBySlot(0, mat);
	}

	AddCollision({ GetTransform().position, glm::vec3(10.0f, 15.0f, 10.0f) }, true);
}

void Star::OnTick(float deltaTime)
{
	SWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 100.0f * deltaTime;
}

void Star::OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& col, const TShared<SSTCollision>& otherCol)
{
	if (otherCol->type == SECollisionTypes::PLAYER)
	{
		SDebug::Log("Star Overlap with Player");
		col->debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		//Destroy();
	}
}
