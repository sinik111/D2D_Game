#include "../D2DEngineLib/framework.h"
#include "Moon.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

void Moon::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    textRenderer->SetText(L"До");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(60.0f);
    textRenderer->SetRectSize({ 60.0f, 60.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Moon"));

    GetTransform()->SetPosition(Vector2(100.0f, 0.0f));

    m_speed = 0.5f;
}

void Moon::Update()
{
    GetTransform()->Rotate(m_speed);
}
