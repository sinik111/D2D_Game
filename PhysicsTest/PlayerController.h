#pragma once

#include "../D2DEngineLib/Script.h"

class RigidBody2D;

class PlayerController :
	public Script
{
private:
	RigidBody2D* m_rb2d = nullptr;

private:
	void Start() override;
	void FixedUpdate() override;
};