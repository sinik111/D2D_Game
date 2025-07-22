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

	assert(m_bitmapRenderer != nullptr && "Button이 참조할 BitmapRenderer가 없습니다");
}

bool Button::IsContainPoint(const Vector2& point)
{
	return m_bitmapRenderer->GetBounds().IsContain(point);
}

SpaceType Button::GetSpaceType() const
{
	return m_bitmapRenderer->GetSpaceType();
}

void Button::InvokeAction()
{
	m_onClickButton.Invoke();
}

void Button::SetClickedColor(const D2D1_COLOR_F& color)
{
	m_bitmapRenderer->SetColor(color);
}

void Button::ResetColor()
{
	m_bitmapRenderer->ResetColor();
}