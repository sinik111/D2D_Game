#pragma once

#include <d2d1_1.h>

class Transform
{
private:
	Transform* m_parent;
	D2D1_VECTOR_2F m_position;
	float m_rotation;
	D2D1_VECTOR_2F m_scale;

	bool m_isDirty;

public:
	Transform();

public:
	D2D1_VECTOR_2F GetPosition() const;
	float GetRotation() const;
	D2D1_VECTOR_2F GetScale() const;
	D2D1::Matrix3x2F GetMatrix() const;
	Transform* GetParent() const;

public:
	void SetPosition(float x, float y);
	void SetPosition(D2D1_VECTOR_2F position);
	void SetRotation(float angle);
	void SetScale(float x, float y);
	void SetScale(D2D1_VECTOR_2F scale);
	void SetParent(Transform* parent);

public:
	void Reset();

public:
	void Translate(float x, float y);
	void Translate(D2D1_VECTOR_2F movement);
	void Rotate(float angle);
};