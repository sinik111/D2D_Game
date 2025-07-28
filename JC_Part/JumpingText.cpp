#include "../D2DEngineLib/framework.h"
#include "JumpingText.h"

#include "../D2DEngineLib/TextRenderer.h"

void JumpingText::Initialize()
{
	auto textRenderer = GetGameObject()->AddComponent<TextRenderer>();

	textRenderer->SetText(L"มกวม!");
	textRenderer->SetFontSize(10.f);
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	textRenderer->SetRectSize({ 40.0f, 20.0f });
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetSortOrder(10);
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
}

void JumpingText::Update()
{
	if (m_gettingBigger)
	{
		Vector2 scale = GetTransform()->GetLocalScale();

		if (scale.x > 2.0f)
		{
			m_gettingBigger = false;
		}

		scale.x += MyTime::DeltaTime() * 2.0f;
		scale.y += MyTime::DeltaTime() * 2.0f;

		GetTransform()->SetLocalScale(scale);
	}
	else
	{
		Vector2 scale = GetTransform()->GetLocalScale();

		scale.x -= MyTime::DeltaTime() * 2.0f;
		scale.y -= MyTime::DeltaTime() * 2.0f;

		if (scale.x < 0.1f)
		{
			Destroy(GetGameObject());

			return;
		}

		GetTransform()->SetLocalScale(scale);
	}
}