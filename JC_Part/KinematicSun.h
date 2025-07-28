#pragma once

#include "../D2DEngineLib/Script.h"

class RigidBody2D;

class KinematicSun :
	public Script
{
private:
	RigidBody2D* m_rigidBody;

private:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
};

