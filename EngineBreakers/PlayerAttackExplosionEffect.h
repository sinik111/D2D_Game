#pragma once

#include "../D2DEngineLib/Script.h"

class Animator;

class PlayerAttackExplosionEffect :
	public Script
{
private:
	Animator* m_animator = nullptr;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;
};