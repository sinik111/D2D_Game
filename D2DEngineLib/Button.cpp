#include "pch.h"
#include "Button.h"

#include "ComponentSystem.h"
#include "GameObject.h"
#include "BitmapRenderer.h"

void Button::RegisterToSystem()
{
	ComponentSystem::Get().Button().RegisterButton(this);
}

void Button::UnregisterFromSystem()
{
	ComponentSystem::Get().Button().UnregisterButton(this);
}

void Button::Initialize()
{
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();

	assert(m_bitmapRenderer != nullptr && "Button�� ������ BitmapRenderer�� �����ϴ�");
}

bool Button::IsContainPoint(const Vector2& point)
{
	return m_bitmapRenderer->GetBounds().IsContain(point);
}
