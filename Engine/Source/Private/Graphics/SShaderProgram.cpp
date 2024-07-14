#include "Graphics/SShaderProgram.h"
#include "Debugging/SDebug.h"
#include "Math/SSTTransform.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTCamera.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

// System Libs
#include <fstream>
#include <sstream>

#define SGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));

SShaderProgram::SShaderProgram()
{
	m_programID = 0;
}

SShaderProgram::~SShaderProgram()
{
	SDebug::Log("Shader program " + std::to_string(m_programID) + "destroyed");
}

bool SShaderProgram::InitShader(const SString& vShaderPath, const SString& fShaderPath)
{
	// create the shader program in open gl
	m_programID = glCreateProgram();

	// test if the create progam failed
	if (m_programID == 0) {
		const std::string errorMsg = SGET_GLEW_ERROR;
		SDebug::Log("Shader failed to initialise, couldn't crate program: " + errorMsg);
		return false;
	}
	
	// if either of the shaders fail to import fail the whole program
	if (!ImportShaderByType(vShaderPath, ST_VERTEX) || !ImportShaderByType(fShaderPath, ST_FRAGMENT)) {
		SDebug::Log("Shader program failed initialise, couldn't import shaders");
		return false;
	}

	return LinkToGPU();
}

void SShaderProgram::Activate()
{
	glUseProgram(m_programID);
}

void SShaderProgram::SetModelTransform(const SSTTransform& transform)
{
	// translate (move) > route > scale (this allows us to rotate around the new location)
	// initialise a default matrix transform
	glm::mat4 matrixT = glm::mat4(1.0f);

	// translate the matrix
	matrixT = glm::translate(matrixT, transform.position);

	// rotate
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	// scale the matrix
	matrixT = glm::scale(matrixT, transform.scale);

	// find the variable in the shader
	// all uniform variables are given an ID by gl
	const int varID = glGetUniformLocation(m_programID, "model");
	// update the value
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT)
	);
 }

void SShaderProgram::SetWorldTransform(const TShared<SSTCamera>& camera)
{
	// initialise a matrix
	glm::mat4 matrixT = glm::mat4(1.0f);

	// handle the view matrix

	// translate the matrix based on camera position
	matrixT = glm::translate(matrixT, camera->transform.position);

	// find the variable in the shader and update it
	int varID = glGetUniformLocation(m_programID, "view");

	// update the value
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT)
	);

	// handle the projection matrix

	// set the projection matrix to a perpective view
	matrixT = glm::perspective(
		glm::radians(camera->fov), // the zoom of your camera
		camera->aspectRatio, // how wide the view is
		camera->nearClip, // how close you can see 3D models
		camera->farClip // how far you can see 3D models - all other models will not render
	);

	// find the variable in the shader for the projection matrix
	varID = glGetUniformLocation(m_programID, "projection");

	// update the project matrix in the shader
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT)
	);


}

void SShaderProgram::RunTexture(const TShared<STexture>& texture, const SUi32& slot)
{
	// Bind the texture
	texture->BindTexture(slot);

	// the id for the variable in the shader
	int varID = 0;

	// get the id depending on the slot
	switch (slot) {
	case 0:
		varID = glGetUniformLocation(m_programID, "colourMap");
		break;
	default:
		break;
	}

	// update the shader
	glUniform1i(varID, slot);

}

bool SShaderProgram::ImportShaderByType(const SString& filePath, SEShaderType shaderType)
{
	// convert the shader to a string
	const SString shaderStr = ConvertFileToString(filePath);

	// make sure there is a string path
	if (shaderStr.empty()) {
		// error that the string failed to import
		SDebug::Log("Shader failed to inport", ST_ERROR);
		return false;
	}

	// set and create id for shader based on shader type
	switch (shaderType)
	{
	case ST_VERTEX:
		m_shaderIDs[shaderType] = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ST_FRAGMENT:
		m_shaderIDs[shaderType] = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}
	
	// make sure there is a string path
	if (m_shaderIDs[shaderType] == 0) {
		// error that the string failed to import
		const SString errorMsg = SGET_GLEW_ERROR;
		SDebug::Log("shader program could not assign shader id: " + errorMsg, ST_ERROR);
		return false;
	}

	// compile the shader onto the GPU
	const char* shaderCStr = shaderStr.c_str();
	glShaderSource(m_shaderIDs[shaderType], 1, &shaderCStr, nullptr);
	glCompileShader(m_shaderIDs[shaderType]);

	// test if the compile worked
	GLint success;
	glGetShaderiv(m_shaderIDs[shaderType], GL_COMPILE_STATUS, &success);

	if (!success) {
		// create an empty log
		char infoLog[512];
		// fill the log with info from gl about what happened
		glGetShaderInfoLog(m_shaderIDs[shaderType], 512, nullptr, infoLog);
		// log it
		SDebug::Log("Shader compilation error: " + SString(infoLog), ST_ERROR);
		return false;
	}

	// attach the shader to the program id
	glAttachShader(m_programID, m_shaderIDs[shaderType]);

	return true;
}

SString SShaderProgram::ConvertFileToString(const SString& filePath)
{
	// convert the file path into an ifstream
	std::ifstream shaderSource(filePath);

	// test if we can open the file
	if (!shaderSource.is_open()) {
		SDebug::Log("Failed to open file: " + filePath, ST_ERROR);
		return {};
	}

	// initilise a sstream
	std::stringstream shaderStream;

	// convert the fstream into an sstream
	// basically turns the file into a string readable by our code
	shaderStream << shaderSource.rdbuf();

	// close the file since we don't need it anymore
	shaderSource.close();

	return shaderStream.str();
}

bool SShaderProgram::LinkToGPU()
{
	// link the program to the GPU
	glLinkProgram(m_programID);

	// test if the compile worked
	GLint success;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	if (!success) {
		// create an empty log
		char infoLog[512];
		// fill the log with info from gl about what happened
		glGetShaderInfoLog(m_programID, 512, nullptr, infoLog);
		// log it
		SDebug::Log("Shader link error: " + SString(infoLog), ST_ERROR);
		return false;
	}

	SDebug::Log("Shader successfully initialised and link at index" + std::to_string(m_programID));

	return true;
}
