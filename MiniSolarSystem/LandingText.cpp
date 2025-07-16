#include "../D2DEngineLib/framework.h"
#include "LandingText.h"

#include "../D2DEngineLib/TextRenderer.h"

void LandingText::Initialize()
{
	auto textRenderer = GetGameObject()->AddComponent<TextRenderer>();

	textRenderer->SetText(L"ÂøÁö!");
	textRenderer->SetFontSize(20.f);
	textRenderer->SetHorizontalAlignment(HorizontlAlignment::Center);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	textRenderer->SetRectSize({ 60.0f, 40.0f });
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetSortOrder(10);
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
}

void LandingText::Update()
{
	Vector2 scale = GetTransform()->GetLocalScale();

	scale.x -= MyTime::DeltaTime() * 1.0f;
	scale.y -= MyTime::DeltaTime() * 1.0f;

	if (scale.x < 0.1f)
	{
		Destroy(GetGameObject());

		return;
	}

	GetTransform()->SetLocalScale(scale);

	if (m_direction.x < 0.0f)
	{
		GetTransform()->Rotate(-360.0f * MyTime::DeltaTime());
	}
	else
	{
		GetTransform()->Rotate(360.0f * MyTime::DeltaTime());
	}

	GetTransform()->Translate(m_direction * MyTime::DeltaTime() * 100.0f);
}

void LandingText::SetDirection(const Vector2& direction)
{
	m_direction = direction;
}
