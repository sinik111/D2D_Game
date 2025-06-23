#pragma once

#include "Matrix3x2.h"
#include "Vector2.h"

#include "Component.h"

class Transform :
	public Component
{
private:
	Vector2 m_position{ 0.0f, 0.0f };
	float m_rotation{ 0.0f };
	Vector2 m_scale{ 1.0f, 1.0f };

	Matrix3x2 m_cachedLocal{ Matrix3x2::Identity() };
	Matrix3x2 m_cachedWorld{ Matrix3x2::Identity() };

	Transform* m_parent{ nullptr };
	std::vector<Transform*> m_children;

	bool m_isLocalDirty{ true };
	bool m_isWorldDirty{ true };

public:
	~Transform() override;

public:
	const Vector2& GetPosition() const;
	float GetRotation() const;
	const Vector2& GetScale() const;
	const Matrix3x2& GetLocalMatrix();
	const Matrix3x2& GetWorldMatrix();
	Transform* GetParent() const;
	const std::vector<Transform*>& GetChildren();

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