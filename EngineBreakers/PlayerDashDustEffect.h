#pragma once

#include "../D2DEngineLib/Script.h"

class Animator;

class PlayerDashDustEffect :
	public Script
{
private:
	Animator* m_animator = nullptr;
	Vector2 m_direction;

private:
	void Initialize() override;
	void Start() override;
	void Update() override;
};