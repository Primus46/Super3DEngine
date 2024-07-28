#pragma once
#include "EngineTypes.h"
#include "Game/SGameEngine.h"

class SObject : public std::enable_shared_from_this<SObject>{
public:
	SObject();
	virtual ~SObject();

	// Run when the object spawns in
	void Start();

	// Run every frame, passes in delta time
	void Tick(float DeltaTime);

	// Run after each tick every frame
	void PostTick(float DeltaTime);

	// mark the object for destroy
	void Destroy();

	// test if the object is marked for destroy
	bool IsPendingDestroy() const { return m_pendingDestroy; }

	// set the lifetime of the object to be destroyed after seconds
	void SetLifeTime(float lifeTime) { 
		m_lifeTime = lifeTime; 
		m_lifeTimeTimer = m_lifeTime;
	}

protected:
	// Run when the object spawns in
	virtual void OnStart() {}

	// Run every frame, passes in delta time
	virtual void OnTick(float DeltaTime) {}

	// Run after each tick every frame
	virtual void OnPostTick(float DeltaTime) {}

private:
	// if marked for destroy
	bool m_pendingDestroy;

	// if set destroy object after value of time
	float m_lifeTime;

	// time before destroyed
	float m_lifeTimeTimer;
};