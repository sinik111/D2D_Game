#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/Matrix3x2.h"

class CameraController
	: public Script
{
private:
	float m_speed;

private:
	void Start() override;
	void Update() override;

	void ProcessInput();
};