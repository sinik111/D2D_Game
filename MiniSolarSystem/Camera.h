#pragma once

#include "../D2DEngineLib/GameObject.h"

class Camera
	: public GameObject
{
private:
	D2D1::Matrix3x2F m_invertedMatrix;

public:
	void Update() override;
	void Render() override;

	void ProcessInput();
	D2D1::Matrix3x2F GetInvertedMatrix();
};