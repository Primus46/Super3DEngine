#include "Game/GameObjects/SWorldObject.h"
#include "Graphics/SGraphicsEngine.h"
#include "Graphics/SModel.h"

TWeak<SModel> SWorldObject::ImportModel(const SString& path)
{
    if(const auto& modelRef = SGameEngine::GetGameEngine()->GetGraphics()->ImportModel(path))
	{
		m_objectModels.push_back(modelRef);
		modelRef->GetTransform() = m_transform;
		return modelRef;
	}

    return {};
	//return TWeak<SModel>();
}

TWeak<SSTCollision> SWorldObject::AddCollision(const SSTBox& box, const bool& debud)
{
	// create the collision
	const TShared<SSTCollision>& newCol = TMakeShared<SSTCollision>();
	// set the position and size
	newCol->box = box;

	// TO DO: DEBUG COLLISION

	// add the collision to the array
	m_objectCollisions.push_back(newCol);

	// return a weak version 
	return newCol;
}

void SWorldObject::TestCollision(const TShared<SWorldObject>& other)
{
	// looping through this objects collisions
	for (const auto& col : m_objectCollisions)
	{
		// loop through the other objects collisions
		for (const auto& otherCol : other->m_objectCollisions)
		{
			if (SSTCollision::IsOverLapping(*col, *otherCol)) 
			{
				OnOverlap(other, otherCol);
			}
		}
	}
}

void SWorldObject::OnPostTick(float deltaTime)
{
	SObject::OnPostTick(deltaTime);

	// all models will have follow the world object
	for (const auto& modelRef : m_objectModels)
	{
		modelRef->GetTransform() = GetTransform();
	}
	
	// all collisions will have follow the world object
	for (const auto& colRef : m_objectCollisions)
	{
		colRef->box.position = GetTransform().position;
	}
}

