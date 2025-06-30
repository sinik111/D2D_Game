#pragma once

#include "Component.h"

class Transform :
	public Component
{
private:
	Vector2 m_localPosition{ 0.0f, 0.0f };
	float m_localRotation = 0.0f;
	Vector2 m_localScale{ 1.0f, 1.0f };

	Vector2 m_worldScale{ 1.0f, 1.0f };
	Vector2 m_worldPosition{ 0.0f, 0.0f };

	Matrix3x2 m_cachedWorld;

	Transform* m_parent = nullptr;
	std::vector<Transform*> m_children;

	bool m_isDirty = true;
	bool m_isScaleDirty = true;
	bool m_isPositionDirty = true;

public:
	~Transform() override;

public:
	const Vector2& GetLocalPosition() const;
	float GetLocalRotation() const;
	const Vector2& GetLocalScale() const;

	const Vector2& GetWorldScale();
	const Vector2& GetWorldPosition();
	const Matrix3x2& GetWorldMatrix();

	Transform* GetParent() const;
	const std::vector<Transform*>& GetChildren();

public:
	void SetLocalPosition(float x, float y);
	void SetLocalPosition(const Vector2& position);
	void SetLocalRotation(float angle);
	void SetLocalScale(float x, float y);
	void SetLocalScale(const Vector2& scale);

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
	void MarkScaleDirty();
	void MarkPositionDirty();
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);
};