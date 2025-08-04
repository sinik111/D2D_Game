#pragma once

#include "../D2DEngineLib/Script.h"

class DummyEnemy :
	public Script
{
private:
	float m_dummyAttackInterval = 3.0f;
	float m_dummyAttackTimer = 0.0f;

private:
	void Update() override;
	void OnTriggerEnter(const Collision& collision) override;
};