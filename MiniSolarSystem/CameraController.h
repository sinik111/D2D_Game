#pragma once

#include "../D2DEngineLib/Script.h"

class CameraController
	: public Script
{
private:
	float m_speed;

public:
	~CameraController() override = default;

private:
	void Start() override;
	void Update() override;

	void ProcessInput();
};