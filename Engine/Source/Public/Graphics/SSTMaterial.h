#pragma once
#include "EngineTypes.h"

class STexture;

struct SSTMaterial {
	SSTMaterial() = default;

	// this is the colour map for the material
	TShared<STexture> m_baseColourMap;

	// this is the shininess of an object
	// the map is black and white
	// white means completely shiny (reflective)
	// reflection of the light
	TShared<STexture> m_specularMap;

	// material properties 
	// shininess of the material
	float shininess = 32.0f;

	// intensity of the specular
	float specularStrength = 0.5f;
};