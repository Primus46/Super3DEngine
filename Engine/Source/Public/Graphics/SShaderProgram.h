#pragma once
#include "EngineTypes.h"

// External Libs
#include <GLM/mat4x4.hpp>


class STexture;
struct SSTCamera;

// enum to determine the type of shader
enum SEShaderType : SUi8 {
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct SSTTransform;
struct SSTLight;
struct SSTMaterial;

class SShaderProgram {
public:
	SShaderProgram();
	~SShaderProgram();

	// create the shader using a vertex and fragment file
	bool InitShader(const SString& vShaderPath, const SString& fShaderPath);

	// activate the shader to update
	// you can't change values in a shader without activating it
	void Activate();

	void SetMeshTransform(const glm::mat4& matTransform);

	// set the transform of the model in the shader
	void SetModelTransform(const SSTTransform& transform);

	// set the 3D coordinates for the model
	void SetWorldTransform(const TShared<SSTCamera>& camera);

	// set the lights in the shader
	void SetLights(const TArray<TShared<SSTLight>>& lights);

	// set the material in the shader
	void SetMaterial(const TShared<SSTMaterial>& material);

	// only works of wireframe shader
	void SetWireColour(const glm::vec3& colour);

private:
	// import a shader based on the shader type
	bool ImportShaderByType(const SString& filePath, SEShaderType shaderType);
	
	// converts a file into a string
	SString ConvertFileToString(const SString& filePath);
	
	// links the shader to the GPU through open gl
	bool LinkToGPU();
private:
	// store the file paths
	SString m_filePath[2] = { "", "" };

	// store the shader ids
	SUi8 m_shaderIDs[2] = { 0, 0 };

	// store the id for the program
	SUi8 m_programID;
};