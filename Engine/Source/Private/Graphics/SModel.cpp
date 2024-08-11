#include "Graphics/SModel.h"

// External Libraries
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/mesh.h>


void SModel::ImportModel(const SString& filePath)
{
	// create the assimp importer
	Assimp::Importer importer;

	// read the file and convert the model to an assimp scene
	// add post prosessing flag triangulate to make sure the model is triangles
	const auto scene = importer.ReadFile(filePath, aiProcess_Triangulate);

	// check if the import failed in any way
	// !scene is checking if the object was null
	// FLAGS_INCOMPLETE is checking if the import failed
	// rootNode is checking if the model has a mesh at all
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		SDebug::Log("Error importing model from " + filePath + ": " + importer.GetErrorString(),
			ST_ERROR);
		return;
	}

	aiMatrix4x4 sceneTransform;
	// update hte scene matrix to start the location at x0, y0, z0
	aiMatrix4x4::Translation({ 0.0f, 0.0f, 0.0f }, sceneTransform);
	// st the rotaton to x0, y0, z0
	sceneTransform.FromEulerAnglesXYZ({ 0.0f, 0.0f, 0.0f });
	// set the scale to x1, y1, z1
	aiMatrix4x4::Scaling({ 1.0f, 1.0f, 1.0f }, sceneTransform);

	// meshes counts
	SUi32 meshesCreated = 0;

	// find all meshes in the scene and fail if any of them fail
	if (!FindAndImportMeshes(*scene->mRootNode, *scene, sceneTransform, &meshesCreated)) {
		SDebug::Log("Model failed to convert ASSIMP scene " + filePath, ST_ERROR);
		return;
	}

	// set the material stack size to the amount of materials on the model
	m_materialsStack.resize(scene->mNumMaterials);

	// log the success of the model
	SDebug::Log("Model successfully imported with " + std::to_string(meshesCreated) + " meshes: "
		+ filePath, ST_SUCCESS);
}

void SModel::Render(const TShared<SShaderProgram>& shader, const TArray<TShared<SSTLight>>& lights)
{
	for(const auto& mesh : m_meshStack) {
		mesh->Render(shader, m_transform, lights, m_materialsStack[mesh->materialIndex]);
	}
}

void SModel::SetMaterialBySlot(unsigned int slot, const TShared<SSTMaterial>& material)
{
	// ensure that the material slot exists
	if (slot >= m_materialsStack.size()) {
		SDebug::Log("No material slot exists at that index: " + std::to_string(slot), ST_WARN);
		return;
	}

	// change the material if it does
	m_materialsStack[slot] = material;
}

bool SModel::FindAndImportMeshes(const aiNode& node, const aiScene& scene,
	const aiMatrix4x4& parentTransform, SUi32* meshesCreated)
{
	// looping through all the meshes in the node
	for (SUi32 i = 0; i < node.mNumMeshes; ++i) {
		// TO DO: convert aiMesh to SMesh

		// getting the mesh index for the mesh in the scene
		const auto aMesh = scene.mMeshes[node.mMeshes[i]];

		// store the mesh vertices and indices
		TArray<SSTVertexData> meshVertices;
		TArray<SUi32> meshIndices;

		// loop through every vertex and get the data for conversion
		for (SUi64 j = 0; j < aMesh->mNumVertices; ++j) {
			// create an empty vertex
			SSTVertexData vertex;
			
			// get the position of the vertex
			vertex.m_position[0] = aMesh->mVertices[j].x;
			vertex.m_position[1] = aMesh->mVertices[j].y;
			vertex.m_position[2] = aMesh->mVertices[j].z;

			// if there are vertex colours then update
			if(aMesh->HasVertexColors(j)) {
				vertex.m_colour[0] = aMesh->mColors[j]->r;
				vertex.m_colour[1] = aMesh->mColors[j]->g;
				vertex.m_colour[2] = aMesh->mColors[j]->b;
			}

			// set the texture coordinates
			// texture coordinates can have multiple sets
			// the first array index is the set [0]
			// the second array index is the vertex data
			if (aMesh->HasTextureCoords(0)) {
				vertex.m_texCoords[0] = aMesh->mTextureCoords[0][j].x;
				vertex.m_texCoords[1] = aMesh->mTextureCoords[0][j].y;
			}

			// get the normal for the vertex
			vertex.m_normal[0] = aMesh->mNormals[j].x;
			vertex.m_normal[1] = aMesh->mNormals[j].y;
			vertex.m_normal[2] = aMesh->mNormals[j].z;

			// add the data into our vertex array
			meshVertices.push_back(vertex);

		}

		// the GPU requires a minimum of 3 vertices to render
		if (meshVertices.size() < 3) {
			SDebug::Log("Mesh has less than 3 vertices", ST_ERROR);
			return false;
		}

		// loop through all of the faces on the mesh to get the indices
		for (SUi64 j = 0; j < aMesh->mNumFaces; ++j) {
			// store the face as a variable
			 auto face = aMesh->mFaces[j];

			// loop through all of the indices in the face
			// there should only be 3
			for (SUi64 k = 0; k < face.mNumIndices; ++k) {
				meshIndices.push_back(face.mIndices[k]);
			}
		}

		// create the mesh object
		auto sMesh = TMakeUnique<SMesh>();


		// test if the mesh fails to create
		if (!sMesh->CreateMesh(meshVertices, meshIndices)) {
			SDebug::Log("Mesh failed to convert from A Mesh to S Mesh", ST_ERROR);
			return false;
		}

		// get the material index from the assimp mesh and set our mesh index to the same
		sMesh->materialIndex = aMesh->mMaterialIndex;

		// set the relative transform for the mesh
		aiMatrix4x4 relTransform = parentTransform * node.mTransformation;

		// set a defualt matrix transform for glm
		glm::mat4 matTransform(1.0f);

		// convert the relative ASSIMP transform into a glm transform
		matTransform[0][0] = relTransform.a1; matTransform[1][0] = relTransform.a2;
		matTransform[2][0] = relTransform.a3; matTransform[3][0] = relTransform.a4;

		matTransform[0][1] = relTransform.b1; matTransform[1][1] = relTransform.b2;
		matTransform[2][1] = relTransform.b3; matTransform[3][1] = relTransform.b4;

		matTransform[0][2] = relTransform.c1; matTransform[1][2] = relTransform.c2;
		matTransform[2][2] = relTransform.c3; matTransform[3][2] = relTransform.c4;

		matTransform[0][3] = relTransform.d1; matTransform[1][3] = relTransform.d2;
		matTransform[2][3] = relTransform.d3; matTransform[3][3] = relTransform.d4;

		// update the relative transform on the mesh
		sMesh->SetRelativeTransform(matTransform);

		// add the new mesh to the mesh stack
		m_meshStack.push_back(std::move(sMesh));

		// count the meshes created
		++*meshesCreated;
	}

	// adding the relative transoform to the parent transform
	const aiMatrix4x4 nodeRelTansform = parentTransform * node.mTransformation;

	// loop through all of the children nodes inside this node
	for (SUi32 i = 0; i < node.mNumChildren; ++i) {
		// recursively call this function to find all of the meshes in the children nodes
		if (!FindAndImportMeshes(*node.mChildren[i], scene, nodeRelTansform, meshesCreated)) {
			return false;
		}
	}

	return true;
}


