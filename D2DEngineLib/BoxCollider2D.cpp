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
		// ����� AABB�� �����ϵ��� ȸ��, �������� ������
		// �������� ȸ������ �и��ؾ��ϱ� ������ �׳� ��.

		//m_bounds = Bounds(GetGameObject()->GetTransform()->G)
	}
}