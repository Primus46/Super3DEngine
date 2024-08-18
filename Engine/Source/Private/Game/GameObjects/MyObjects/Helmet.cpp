#include "Game/GameObjects/MyObjects/Helmet.h"
#include "Graphics/SModel.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTMaterial.h"

Helmet::Helmet()
{
	
}

void Helmet::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Helmet3/Helmet3.fbx").lock())
	{
		// creating a texture
		TShared<STexture> tex = TMakeShared<STexture>();
		tex->LoadTexture("head base colour", "Models/Helmet3/Textures/Head_Base_color.png");

		// creating a specular texture
		TShared<STexture> specTex = TMakeShared<STexture>();
		specTex->LoadTexture("head spec colour", "Models/Helmet3/Textures/Head_Specular.png");

		// creating a second texture
		TShared<STexture> tex2 = TMakeShared<STexture>();
		tex2->LoadTexture("face texture base colour", "Models/Helmet3/Textures/facetexture_Base_color.png");

		// creating a second specular texture
		TShared<STexture> specTex2 = TMakeShared<STexture>();
		specTex2->LoadTexture("face texture spec colour", "Models/Helmet3/Textures/facetexture_Specular.png");

		// creating a material
		TShared<SSTMaterial> mat = TMakeShared<SSTMaterial>();
		TShared<SSTMaterial> mat2 = TMakeShared<SSTMaterial>();
		// assigning the texture to the base colour map for the material
		mat->m_baseColourMap = tex;
		mat->m_specularMap = specTex;

		mat2->m_baseColourMap = tex2;
		mat2->m_specularMap = specTex2;

		// setting the material to the 0 slot in the model
		modelRef->SetMaterialBySlot(0, mat);
		modelRef->SetMaterialBySlot(1, mat2);
	}

	AddCollision({ GetTransform().position, glm::vec3(10.0f, 15.0f, 10.0f) }, true);


	/*if (const auto& modelRef = ImportModel("Models/Helmet3/Helmet3.fbx").lock()) {
		modelRef->m_offset.position.x = 10.0f;
	}
	if (const auto& modelRef = ImportModel("Models/Helmet3/Helmet3.fbx").lock()) {
		modelRef->m_offset.position.x = -10.0f;
	}*/
}

void Helmet::OnTick(float deltaTime)
{
	SWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 10.0f * deltaTime;
}

void Helmet::OnOverlap(const TShared<SWorldObject>& other, const TShared<SSTCollision>& col, const TShared<SSTCollision>& otherCol)
{
	if (otherCol->type == SECollisionTypes::PLAYER)
	{
		SDebug::Log("Helmet Overlapped with Player");
		col->debugColour = glm::vec3(0.0f, 1.0f, 0.0f);
		//Destroy();
	}
}


