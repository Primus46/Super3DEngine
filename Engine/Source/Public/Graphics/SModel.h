#pragma once

#include "EngineTypes.h"
#include "Graphics/SMesh.h"
#include "Math/SSTTransform.h"

class STexture;
class SShaderProgram;

class SModel {
public:
	SModel() = default;
	~SModel() = default;

	// create a polygon model and add a texture to it
	void MakePoly(const TShared<STexture>& texture);

	// create a cube model and add a texture to it
	void MakeCube(const TShared<STexture>& texture);

	// render all of the meshes within the model
	// transform of meshes will be based on the model transformations
	void Render(const TShared<SShaderProgram>& shader);

	// get the transform of the model
	SSTTransform& GetTransform() { return m_transform; }

private:
	// array of meshes
	TArray <TUnique<SMesh>> m_meshStack;

	// transfor for the model in 3D space
	SSTTransform m_transform;
};