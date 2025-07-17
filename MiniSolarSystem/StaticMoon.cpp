#include "../D2DEngineLib/framework.h"
#include "StaticMoon.h"

#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/TextRenderer.h"

void StaticMoon::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"KinematicSun")
	{
		GameObject* go = CreateGameObject();
		go->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
		auto textRenderer = go->AddComponent<TextRenderer>();
		textRenderer->SetText(L"ÆÄ±«!");
		textRenderer->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		textRenderer->SetFontSize(15.0f);
		textRenderer->SetFontSize(15.0f);
		textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
		textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
		Destroy(go, 1.0f);

		Destroy(GetGameObject());
	}
}
