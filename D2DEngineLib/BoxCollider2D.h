#pragma once

#include "Component.h"

class BoxCollider2D :
	public Component
{
private:
	Bounds m_bounds;
	Vector2 m_offset;
	Vector2 m_size;
	bool m_isBoundsDirty = true;

public:
	BoxCollider2D() = default;
	~BoxCollider2D() override = default;

public:
	void SetOffset(const Vector2& offset);
	void SetSize(const Vector2& size);

public:
	void Update();
};