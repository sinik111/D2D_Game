#pragma once

#include "../D2DEngineLib/Script.h"

class CameraController
	: public Script
{
private:
	Vector2 m_direction;
	float m_speed = 0.0f;
	Vector2 prevPos;
	Vector2 position{ 0,0 };
private:
	void Start() override;
	void Update() override;

	void MakeDirection(Vector2 input);

	void ZoomIn();
	void ZoomOut();
	void ChangeScene();
};