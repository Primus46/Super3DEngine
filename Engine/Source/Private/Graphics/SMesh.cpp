#include "Graphics/SMesh.h"
#include "Debugging/SDebug.h"

// External Libs
#include <GLEW/glew.h>

SMesh::SMesh()
{
	m_vao = m_vbo = m_eao = 0;
	SDebug::Log("Mesh created");
}

SMesh::~SMesh()
{
	SDebug::Log("Mesh destroyed");
}

bool SMesh::CreateMesh(const std::vector<SSTVertexData> vertices, const std::vector<uint32_t>& indices)
{
	// store the vertex data
	m_vertices = vertices;
	m_indices = indices;

	// create a vertex array object (VAO)
	// assign the ID for the object to the m_vao variable
	// stores a reference to any VBO's attached to the VAO
	glGenVertexArrays(1, &m_vao);

	// test if the vao failed
	if (m_vao == 0) {
		// convert the error int a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		SDebug::Log("Mesh failed to create VAO: " + errorMsg, ST_WARN);
		return false;
	}

	// this says use the vao as the active working vao for any vao functions
	glBindVertexArray(m_vao);

	// create a vertex buffer object
	// Vertex Buffer Object holds the data for the vertices in the GPU
	glGenBuffers(1, &m_vbo);

	// test if the vbo failed
	if (m_vbo == 0) {
		// convert the error int a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		SDebug::Log("Mesh failed to create VBO: " + errorMsg, ST_WARN);
		return false;
	}

	// bind the buffer object to say this is the active working vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// create a Element Array Object
	glGenBuffers(1, &m_eao);

	// test if the vbo failed
	if (m_eao == 0) {
		// convert the error int a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		SDebug::Log("Mesh failed to create EAO: " + errorMsg, ST_WARN);
		return false;
	}

	// bind the eao as the active element array buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_eao);

	// set the buffer data
	// start with the VBO which stores the vertex data
	glBufferData(
		GL_ARRAY_BUFFER, // type of data that we're storing
		static_cast<GLsizeiptr>(m_vertices.size() * sizeof(SSTVertexData)), // size of each data in bytes
		m_vertices.data(), // memory location of the data
		GL_STATIC_DRAW // this data will not be modified frequently
	);

	// set the data for the EAO
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(m_indices.size() * sizeof(uint32_t)),
		m_indices.data(),
		GL_STATIC_DRAW
	);

	// pass out the vertex data in seperate formats
	glDisableVertexAttribArray(0);

	// set the position of that data to the 0 index of the attribute array
	glVertexAttribPointer(
		0, // location to store data in the atttribute array
		3, // how many numbers to pass into the attribute array index
		GL_FLOAT, // the type of data to store
		GL_FALSE, // should we normalise the valuse, generally no
		sizeof(SSTVertexData), // how big is each data array in a vertex
		nullptr // 
	);

	// common practice to clear the vao from the GPU
	glBindVertexArray(0);

	return true;

}

void SMesh::Render()
{
	// binding this mesh as the active vao
	glBindVertexArray(m_vao);

	// render the vao
	glDrawElements(
		GL_TRIANGLES, // draw the mesh as triangles
		static_cast<GLsizei>(m_indices.size()), // how many vertices are there?
		GL_UNSIGNED_INT, // what type of data is the index array
		nullptr // how many are you going to skip
	);

	// clear the vao
	glBindVertexArray(0);
}
