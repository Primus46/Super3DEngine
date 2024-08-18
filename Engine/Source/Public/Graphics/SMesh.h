#pragma once
#include "EngineTypes.h"

// External Libs
#include <GLM/mat4x4.hpp>


class SShaderProgram;
struct SSTTransform;
struct SSTLight;
struct SSTMaterial;

struct SSTVertexData {
	// 0 = x, 
	// 1 = y, 
	// 2 = z
	float m_position[3] = { 0.0f, 0.0f, 0.0f };

	// 0 = r,
	// 1 = g,
	// 2 = b
	float m_colour[3] = { 1.0f, 1.0f, 1.0f };

	// 0 = x, u, s
	// 1 = y, v, t
	float m_texCoords[2] = { 0.0f, 0.0f };

	// 0 = x,
	// 1 = y,
	// 2 = z
	float m_normal[3] = { 0.0f, 0.0f, 0.0f };
};

class SMesh {
public:
	SMesh();
	~SMesh();
	
	// creating a mesh using vertex and index data
	bool CreateMesh(const std::vector<SSTVertexData>& vertices, const std::vector<uint32_t>& indices);

	// draw the mesh to the renderer
	void Render(const std::shared_ptr<SShaderProgram>& shader, const SSTTransform& transform, 
		const TArray<TShared<SSTLight>>& lights, const TShared<SSTMaterial>& material);

	// draw a wireframe of a mesh
	void Wireframe(const TShared<SShaderProgram>& shader, const SSTTransform& transform);

	// set the transform of the mesh relative to the model
	void SetRelativeTransform(const glm::mat4& transform) { m_matTransform = transform; }

public:
	// the index for the material relative to the model
	unsigned int materialIndex;

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
	uint32_t m_eab;

	// relative transform of the mesh
	glm::mat4 m_matTransform;
};