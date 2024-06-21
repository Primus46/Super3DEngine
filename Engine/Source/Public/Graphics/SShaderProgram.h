#pragma once
#include "EngineTypes.h"


// System Libs
#include <string>
#include <cstdint>

// enum to determine the type of shader
enum SEShaderType : uint8_t {
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct  SSTTransform;

class SShaderProgram {
public:
	SShaderProgram();
	~SShaderProgram();

	// create the shader using a vertex and fragment file
	bool InitShader(const SString& vShaderPath, const SString& fShaderPath);

	// activate the shader to update
	// you can't change values in a shader without activating it
	void Activate();

	// set teh transform of the model in the shader
	void SetModelTransform(const SSTTransform& transform);

	// set a texture in the shader based on the shot
	void RunTexture(const TShared<SString>& texture, const SUi32& slot);

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
	uint32_t m_shaderIDs[2] = { 0, 0 };

	// store the id for the program
	uint32_t m_programID;
};