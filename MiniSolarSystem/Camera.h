#pragma once

#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/Matrix3x2.h"

class Camera
	: public GameObject
{
private:
	Matrix3x2 m_invertedMatrix;

public:
	void Update() override;
	void Render() override;

	void ProcessInput();
	Matrix3x2 GetInvertedMatrix();
};