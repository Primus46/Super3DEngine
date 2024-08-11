#pragma once

#include "EngineTypes.h"
#include "Graphics/SMesh.h"
#include "Math/SSTTransform.h"

// External Libs
#include <ASSIMP/matrix4x4.h>

class STexture;
class SShaderProgram;
struct aiScene;
struct aiNode;
struct SSTLight;
struct SSTMaterial;

class SModel {
public:
	SModel() = default;
	~SModel() = default;


	// import a 3D model from a file
	// uses the ASSIMP import library, check docs to know which file types accepted
	void ImportModel(const SString& filePath);

	// render all of the meshes within the model
	// transform of meshes will be based on the model transformations
	void Render(const TShared<SShaderProgram>& shader, const TArray<TShared<SSTLight>>& lights);

	// get the transform of the model
	SSTTransform& GetTransform() { return m_transform; }

	// set a material by the slot number
	void SetMaterialBySlot(unsigned int slot, const TShared<SSTMaterial>& material);

private:
	// find all of the meshes in a scene and convert them to a SMesh
	bool FindAndImportMeshes(const aiNode& node, const aiScene& scene, 
		const aiMatrix4x4& parentTransform, SUi32* meshesCreated);

private:
	// array of meshes
	TArray <TUnique<SMesh>> m_meshStack;

	// array of materials for the model
	TArray<TShared<SSTMaterial>> m_materialsStack;

	// transfor for the model in 3D space
	SSTTransform m_transform;
};