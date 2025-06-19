#include "../D2DEngineLib/framework.h"
#include "Sun.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

void Sun::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();
    
    textRenderer->SetText(L"еб╬Г");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(20.0f);
    textRenderer->SetRectSize({ 60.0f, 60.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Sun"));

    GetTransform()->SetPosition(Vector2(0.0f, 0.0f));

    m_speed = 0.2f;
}

void Sun::Update()
{
    GetTransform()->Rotate(m_speed);
}
