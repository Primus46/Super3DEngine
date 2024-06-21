#include "Graphics/STexture.h"

// External Libs
#include "GLEW/glew.h"
#include "STB_IMAGE/stb_image.h"

STexture::STexture()
{
	m_path = m_fileName = "";
	m_ID = 0U;
	m_width = m_height = m_channels = 0;
}

STexture::~STexture()
{
	// as long as an id was generated, delete the texture from open gl
	if(m_ID > 0)
		glDeleteTextures(1, &m_ID);

	SDebug::Log("Texture destroyed: " + m_fileName);
}

bool STexture::LoadTexture(const SString& fileName, const SString& path)
{
	// assign the file name and path
	m_fileName = fileName;
	m_path = path;

	// std image imports images upside down
	// but actually open gl reads them in an inverted state
	stbi_set_flip_vertically_on_load(true);

	// load the image into a computer readable format
	unsigned char* data = stbi_load(
		m_path.c_str(), // path to the image
		&m_width, &m_height, // wisth and height of the image
		&m_channels, // rgba
		0 // do we want to specifically require a certain amount of channels, 0 = no limit
	);

	//  test if data imported or not
	if (data == nullptr) {
		SString error = "Failed to load texture - " + fileName + ": " + stbi_failure_reason();
		SDebug::Log(error, ST_ERROR);
		return false;
	}

	// test the amount of channels
	if (m_channels > 4 || m_channels < 3) {
		SDebug::Log("Failed to import texture - " + m_fileName 
			+ ": Incorrect number of channels");
		return false;
	}

	// generate the texture ID in open gl
	glGenTextures(1, &m_ID);

	// test if the generate failed
	if (m_ID == 0) {
		SString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		SString errorMsg = "Failed to load texture - " + fileName + ": " + error;
		SDebug::Log(errorMsg, ST_ERROR);
		return false;
	}

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// set some default parameters for the texture
	// set the texture wrapping parameters
	// if the texture doesn't fit the model, repeat the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y

	// set the filtering parameters
	// how much to blue pixels
	// the resolution of the texture is lower than the size of the model
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set the default format at 3 channels
	GLint intFormat = GL_RGB;

	// if the channels of the imported image = 4 then set the import type to rgba
	if (m_channels == 4) {
		intFormat = GL_RGBA;
	}

	// load the image data into the texture that we just updated
	glTexImage2D(
		GL_TEXTURE_2D, // use a 2D texture
		0, // levels
		intFormat, // format of the texture
		m_width, m_height, // width and height
		0, // border around the image (legacy)
		intFormat, // format of the texture (external)
		GL_UNSIGNED_BYTE, // type of data passed in
		data // image we got form stbi
	);
	
	// generate mip maps
	// lower resolution versions of the texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// unbind the texture from open gl
	// make room for next texture
	Unbind();

	// clear stbi image data
	stbi_image_free(data);

	// log the success of the import
	SDebug::Log("Successfully imported texture - " + m_fileName, ST_SUCCESS);

	return true;
}

void STexture::BindTexture(const SUi32& textureNumber)
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void STexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
