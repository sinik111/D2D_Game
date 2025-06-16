#pragma once

#include "Matrix3x2.h"
#include "Vector2.h"

#include "Component.h"

class Transform :
	public Component
{
private:
	Vector2 m_position;
	float m_rotation;
	Vector2 m_scale;

	Matrix3x2 m_cachedLocal;
	Matrix3x2 m_cachedWorld;

	Transform* m_parent;
	std::vector<Transform*> m_children;

	bool m_isLocalDirty;
	bool m_isWorldDirty;

public:
	Transform();
	~Transform() override;

public:
	const Vector2& GetPosition() const;
	float GetRotation() const;
	const Vector2& GetScale() const;
	const Matrix3x2& GetLocalMatrix();
	const Matrix3x2& GetWorldMatrix();
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

private:
	void MarkWorldDirty();
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);
};