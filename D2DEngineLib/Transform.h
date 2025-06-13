#pragma once

#include "Matrix3x2.h"
#include "Vector2.h"

class Transform
{
private:
	Transform* m_parent;
	Vector2 m_position;
	float m_rotation;
	Vector2 m_scale;

	bool m_isDirty;

public:
	Transform();

public:
	Vector2 GetPosition() const;
	float GetRotation() const;
	Vector2 GetScale() const;
	Matrix3x2 GetWorldMatrix() const;
	Transform* GetParent() const;

public:
	void SetPosition(float x, float y);
	void SetPosition(const Vector2& position);
	void SetRotation(float angle);
	void SetScale(float x, float y);
	void SetScale(const Vector2& scale);
	void SetParent(Transform* parent);

public:
	void Reset();

public:
	void Translate(float x, float y);
	void Translate(const Vector2& movement);
	void Rotate(float angle);
};