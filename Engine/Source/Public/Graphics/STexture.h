#pragma once
#include "EngineTypes.h"

class STexture {
public:
	STexture();
	~STexture();

	// import a file and convert it to a texture
	bool LoadTexture(const SString& fileName, const SString& path);

	// activates the texture to use in open gl
	void BindTexture(const SUi32& textureNumber);

	// deactivates the texture in open gl
	void Unbind();

	// gets the import path of the texture
	SString GetImmportPath() const { return m_path; }

	// gets the custom file name of the texture
	SString GetName() const { return m_fileName; }

	// get the id of the texture for open gl
	SUi32 GetID() const { return m_ID; }

private:
	// import path to image
	SString m_path;

	// custom name of the texture
	SString m_fileName;

	// the id for the texture in open gl
	SUi32 m_ID;

	// texture parameters
	int m_width, m_height, m_channels;
};