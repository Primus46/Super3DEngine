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

class SModel {
public:
	SModel() = default;
	~SModel() = default;

	// create a polygon model and add a texture to it
	void MakePoly(const TShared<STexture>& texture);

	// create a pyramid model and add a texture to it
	void MakePyramid(const TShared<STexture>& texture);

	// create a cube model and add a texture to it
	void MakeCube(const TShared<STexture>& texture);

	// import a 3D model from a file
	// uses the ASSIMP import library, check docs to know which file types accepted
	void ImportModel(const SString& filePath);

	// render all of the meshes within the model
	// transform of meshes will be based on the model transformations
	void Render(const TShared<SShaderProgram>& shader);

	// get the transform of the model
	SSTTransform& GetTransform() { return m_transform; }

private:
	// find all of the meshes in a scene and convert them to a SMesh
	bool FindAndImportMeshes(const aiNode& node, const aiScene& scene, 
		const aiMatrix4x4& parentTransform);

private:
	// array of meshes
	TArray <TUnique<SMesh>> m_meshStack;

	// transfor for the model in 3D space
	SSTTransform m_transform;
};