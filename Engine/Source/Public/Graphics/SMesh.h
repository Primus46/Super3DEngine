#pragma once

// System Libs
#include <iostream>
#include <vector>
#include <cstdint>

struct SSTVertexData {
	float m_position[3] = { 0.0f, 0.0f, 0.0f };
	float m_colour[3] = { 1.0f, 1.0f, 1.0f };
	float m_texCoords[2] = { 0.0f, 0.0f };
	float m_bormal[3] = { 0.0f, 0.0f, 0.0f };
};

class SMesh {
public:
	SMesh();
	~SMesh();
	
	// creating a mesh using vertex and index data
	bool CreateMesh(const std::vector<SSTVertexData> vertices, 
		const std::vector<uint32_t>& indices);

	// draw the mesh to the renderer
	void Render();

private:
	// store the vertices
	std::vector<SSTVertexData> m_vertices;

	// store the indices for the vertex data
	std::vector<uint32_t> m_indices;

	// store the ID for the vertex array object
	uint32_t m_vao;

	// store the ID for the vertex buffer object
	uint32_t m_vbo;

	// store the ID for the element array object
	uint32_t m_eao;

};