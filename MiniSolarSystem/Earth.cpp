#include "../D2DEngineLib/framework.h"
#include "Earth.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

void Earth::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    textRenderer->SetText(L"Áö±¸");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(30.0f);
    textRenderer->SetRectSize({ 100.0f, 100.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Earth"));

    GetTransform()->SetScale(Vector2(0.5f, 0.5f));
    GetTransform()->SetPosition(Vector2(200.0f, 0.0f));

    m_speed = 0.3f;
}

void Earth::Update()
{
    GetTransform()->Rotate(m_speed);
}
