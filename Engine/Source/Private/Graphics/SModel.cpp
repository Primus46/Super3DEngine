#include "Graphics/SModel.h"

std::vector<SSTVertexData> polyVData = {
	//	 x	   y	 z        r     g     b	      tx	ty
	// square
	{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },// vertx data 1 - top left
	{ { 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} }, // vertx data 2 - top right
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} }, // vertx data 3 - bottom left
	{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }, // vertx data 4 - bottom right

};

std::vector<uint32_t> polyIData = {
	0, 1, 2, // triangle 1
	1, 2, 3, // triangle 2
};

std::vector<SSTVertexData> pyramidVData = {
	//	 x	   y	 z        r     g     b	      tx	ty
	// pyramid
	
};

std::vector<uint32_t> pyramidIData = {
};


std::vector<SSTVertexData> cubeVData = {
	//	  x      y      z         r     g     b         tx    ty
	//cube
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // f 1 top left - 0
	{ {	 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // f 2 top right - 1
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // f 3 bottom left - 2
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // f 4 bottom right - 3

	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // b 1 top right - 4
	{ {	-1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // b 2 top left - 5
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // b 3 bottom right - 6
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // b 4 bottom left - 7

	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // l 1 top left - 8
	{ {	 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // l 2 top right - 9
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // l 3 bottom left - 10
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // l 4 bottom right - 11

	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // r 1 top right - 12
	{ {	-1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // r 2 top left - 13
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // r 3 bottom right - 14
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // r 4 bottom left - 15

	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // t 1 top left - 16
	{ {	 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // t 2 top right - 17
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // t 3 bottom left - 18
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // t 4 bottom right - 19

	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // bt 1 top right - 20
	{ {	-1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // bt 2 top left - 21
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // bt 3 bottom right - 22
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }  // bt 4 bottom left - 23
};

std::vector<uint32_t> cubeIData = {
	0, 1, 2, // f triangle 1
	1, 2, 3, // f triangle 2

	4, 5, 6, // b triangle 1
	5, 6, 7, // b triangle 2

	13, 8, 10, // l triangle 1
	13, 15, 10, // l triangle 2

	9, 12, 14, // r triangle 1
	9, 14, 11, // r triangle 2

	21, 20, 16, // t triangle 1
	20, 17, 16, // t triangle 2

	18, 19, 23, // bt triangle 1
	23, 19, 22 // bt triangle 2
};

void SModel::MakePoly(const TShared<STexture>& texture)
{
	// create the mesh
	TUnique<SMesh> mesh = TMakeUnique<SMesh>();

	if (!mesh->CreateMesh(polyVData, polyIData)) {
		SDebug::Log("Failed to create mesh");
	}

	// add the  texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// stdmove will move the reference from the previous reference to a new one
	// - without destroying or copying the references
	m_meshStack.push_back(std::move(mesh));
}

void SModel::MakePyramid(const TShared<STexture>& texture)
{
	// create the mesh
	TUnique<SMesh> mesh = TMakeUnique<SMesh>();

	if (!mesh->CreateMesh(pyramidVData, pyramidIData)) {
		SDebug::Log("Failed to create mesh");
	}

	// add the  texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// stdmove will move the reference from the previous reference to a new one
	// - without destroying or copying the references
	m_meshStack.push_back(std::move(mesh));
}

void SModel::MakeCube(const TShared<STexture>& texture)
{
	// create the mesh
	TUnique<SMesh> mesh = TMakeUnique<SMesh>();

	if (!mesh->CreateMesh(cubeVData, cubeIData)) {
		SDebug::Log("Failed to create mesh");
	}

	// add the  texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// stdmove will move the reference from the previous reference to a new one
	// - without destroying or copying the references
	m_meshStack.push_back(std::move(mesh));
}

void SModel::Render(const TShared<SShaderProgram>& shader)
{
	for(const auto& mesh : m_meshStack) {
		mesh->Render(shader, m_transform);
	}
}
