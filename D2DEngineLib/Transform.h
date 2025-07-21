#pragma once

#include "Component.h"

class Transform :
	public Component
{
private:
	Vector2 m_localPosition{ 0.0f, 0.0f };
	Vector2 m_localScale{ 1.0f, 1.0f };
	float m_localRotation = 0.0f;

	Matrix3x2 m_cachedWorld;

	Transform* m_parent = nullptr;
	std::vector<Transform*> m_children;

	bool m_isDirty = true;
	bool m_isDirtyThisFrame = true;

public:
	Transform();
	~Transform() override;

public:
	const Vector2& GetLocalPosition() const;
	const Vector2& GetLocalScale() const;
	float GetLocalRotation() const;

	Vector2 GetWorldPosition();
	Vector2 GetWorldScale();
	float GetWorldRotation();

	const Matrix3x2& GetWorldMatrix();
	void CalculateWorldMatrix();

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

public:
	bool GetIsDirtyThisFrame() const;
	void UnmarkDirtyThisFrame();

private:
	Matrix3x2 MakeLocalMatrix();
	void MarkDirty();
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);
};