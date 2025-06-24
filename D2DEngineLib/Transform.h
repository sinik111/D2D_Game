#pragma once

#include "Component.h"

class Transform :
	public Component
{
private:
	Vector2 m_position{ 0.0f, 0.0f };
	float m_rotation{ 0.0f };
	Vector2 m_scale{ 1.0f, 1.0f };

	Matrix3x2 m_cachedWorld;

	Transform* m_parent{};
	std::vector<Transform*> m_children;

	bool m_isDirty{ true };

public:
	~Transform() override;

public:
	const Vector2& GetPosition() const;
	float GetRotation() const;
	const Vector2& GetScale() const;
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
	Matrix3x2 MakeLocalMatrix();
	void MarkDirty();
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);
};