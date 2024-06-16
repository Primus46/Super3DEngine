#pragma once

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
	bool InitShader(const std::string& vShaderPath, const std::string& fShaderPath);

	// activate the shader to update
	// you can't change values in a shader without activating it
	void Activate();

	// set teh transform of the model in the shader
	void SetModelTransform(const SSTTransform& transform);

private:
	// import a shader based on the shader type
	bool ImportShaderByType(const std::string& filePath, SEShaderType shaderType);
	
	// converts a file into a string
	std::string ConvertFileToString(const std::string& filePath);
	
	// links the shader to the GPU through open gl
	bool LinkToGPU();
private:
	// store the file paths
	std::string m_filePath[2] = { "", "" };

	// store the shader ids
	uint32_t m_shaderIDs[2] = { 0, 0 };

	// store the id for the program
	uint32_t m_programID;
};