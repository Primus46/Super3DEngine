#include "Game/GameObjects/SObject.h"

SObject::SObject()
{
	m_pendingDestroy = false;
	m_lifeTime = 0.0f;
	m_lifeTimeTimer = 0.0f;
	SDebug::Log("SObject Created");
}

SObject::~SObject()
{
	SDebug::Log("SObject Destroyed");
}

void SObject::Start()
{
	OnStart();
}

void SObject::Tick(float DeltaTime)
{
	OnTick(DeltaTime);

	if (m_lifeTime > 0.0f) {
		// countdown the lifetime timer
		m_lifeTimeTimer -= DeltaTime;

		// when the timer hits 0 destroy the object
		if (m_lifeTimeTimer <= 0.0f) {
			Destroy();
		}
	}
}

void SObject::PostTick(float DeltaTime)
{
	OnPostTick(DeltaTime);
}

void SObject::Destroy()
{
	m_pendingDestroy = true;
	SGameEngine::GetGameEngine()->DestroyObject(shared_from_this());
}
