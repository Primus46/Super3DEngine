#include "Graphics/SShaderProgram.h"
#include "Debugging/SDebug.h"
#include "Math/SSTTransform.h"
#include "Graphics/STexture.h"
#include "Graphics/SSTCamera.h"
#include "Graphics/SSTLight.h"
#include "Graphics/SSTMaterial.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

// System Libs
#include <fstream>
#include <sstream>

#define SGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));

// constant value for light amounts
const SUi32 maxDirLights = 2;
const SUi32 maxPointLights = 20;

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

void SShaderProgram::SetMeshTransform(const glm::mat4& matTransform)
{
	// find the variable in the shader
	// all uniform variables are given an ID by gl
	const int varID = glGetUniformLocation(m_programID, "mesh");
	// update the value
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matTransform)
	);
}

void SShaderProgram::SetModelTransform(const SSTTransform& transform)
{
	// translate (move) > route > scale (this allows us to rotate around the new location)
	// initialise a default matrix transform
	glm::mat4 matrixT = glm::mat4(1.0f);

	// translate the matrix
	matrixT = glm::translate(matrixT, transform.position);

	// rotate per axis
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
	// matrixT = glm::translate(matrixT, camera->transform.position);

	// translate the matrix based on camera position
	matrixT = glm::lookAt(
		camera->transform.position,
		camera->transform.position + camera->transform.Forward(),
		camera->transform.Up()

	);

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

void SShaderProgram::SetLights(const TArray<TShared<SSTLight>>& lights)
{
	SUi32 dirLights = 0;
	SUi32 pointLights = 0;
	int varID = 0;
	// Name of the variable array
	// will set in the loop depending on the light type
	SString lightsIndexStr = "";

	// loop through all of the lights and add them to the shader
	for (SUi32 i = 0; i < lights.size(); ++i) {
		if (const auto& lightRef = std::dynamic_pointer_cast<SSTDirLight>(lights[i])) {
			// ignore the light if we have already maxed out 
			if (dirLights >= maxDirLights) {
				continue;
			}

			// add a dir light and use as index
			lightsIndexStr = "dirLights[" + std::to_string(dirLights) + "]";
			
			//_________COLOUR
			// get the colour variable from the dir light struct in the shader
			varID = glGetUniformLocation(m_programID, 
				(lightsIndexStr + ".colour").c_str());

			// change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			//_________AMBIENT
			// get the ambient variable from the dir light struct in the shader
				varID = glGetUniformLocation(m_programID,
					(lightsIndexStr + ".ambient").c_str());

			// change the ambient
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->ambient));

			//_________DIRECTION
			// get the direction variable from the dir light struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".direction").c_str());

			// change the direction
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->direction));

			//_________INTENSITY
			// get the colour intensity from the dir light struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".intensity").c_str());

			// change the intensity
			glUniform1f(varID, lightRef->intensity);

			++dirLights;
			continue;
		}
		
		if (const auto& lightRef = std::dynamic_pointer_cast<SSTPointLight>(lights[i])) {
			// ignore the light if we have already maxed out 
			if (pointLights >= maxPointLights) {
				continue;
			}

			// add a point light and use as index
			lightsIndexStr = "pointLights[" + std::to_string(pointLights) + "]";

			//_________COLOUR
			// get the colour variable from the point light struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".colour").c_str());

			// change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			//_________POSITION
			// get the shader variable id
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".position").c_str());

			// update shader value
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->position));

			//_________INTENSITY
			// get the colour intensity from the point light struct in the shader
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".intensity").c_str());

			// change the intensity
			glUniform1f(varID, lightRef->intensity);

			//_________LINEAR ATTENUATION
			// get the variable id
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".linear").c_str());

			// change the linear value
			glUniform1f(varID, lightRef->linear);

			//_________QUADRATIC ATTENUATION
			// get the variable id
			varID = glGetUniformLocation(m_programID,
				(lightsIndexStr + ".quadratic").c_str());

			// change the quadratic value
			glUniform1f(varID, lightRef->quadratic);

			++pointLights;
		}
	}
}

void SShaderProgram::SetMaterial(const TShared<SSTMaterial>& material)
{
	if(material == nullptr) {
		return;
	}

	// the id for the variable in the shader
	int varID = 0;
	
	//_________BASE COLOUR

	// Bind the texture to the 0 index
	material->m_baseColourMap->BindTexture(0);

	// get base colour map id
	varID = glGetUniformLocation(m_programID, "material.baseColourMap");

	// update the shader
	glUniform1i(varID, 0);

	//_________SPECULAR MAP
	if (material->m_specularMap) {
		// bind the texture to the 1 index
		material->m_specularMap->BindTexture(1);

		// get specular Map id
		varID = glGetUniformLocation(m_programID, "material.specularMap");

		// update the shader
		glUniform1d(varID, 1);
	}

	//_________SHININESS

	// get the base colour map id
	varID = glGetUniformLocation(m_programID, "material.shininess");

	// update the shader
	glUniform1f(varID, material->shininess);

	//_________SPECULAR STRENGTH

	// get the base colour map id
	varID = glGetUniformLocation(m_programID, "material.specularStrength");

	// update the shader
	glUniform1f(varID, material->specularStrength);
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
