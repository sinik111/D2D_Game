#include "pch.h"
#include "BoxCollider2D.h"

#include "GameObject.h"

void BoxCollider2D::SetOffset(const Vector2& offset)
{
	m_offset = offset;

	m_isBoundsDirty = true;
}

void BoxCollider2D::SetSize(const Vector2& size)
{
	m_size = size;

	m_isBoundsDirty = true;
}

void BoxCollider2D::Update()
{
	if (m_isBoundsDirty || GetGameObject()->GetTransform()->GetIsDirtyThisFrame())
	{
		// 현재는 AABB만 가능하도록 회전, 스케일은 제외함
		// 스케일은 회전에서 분리해야하기 때문에 그냥 뺌.

		//m_bounds = Bounds(GetGameObject()->GetTransform()->G)
	}
}