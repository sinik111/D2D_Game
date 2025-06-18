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
    textRenderer->SetFontSize(150.0f);
    textRenderer->SetRectSize({ 300.0f, 300.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::GetBitmap(L"Sun"));

    GetTransform()->SetPosition(Vector2(0.0f, 0.0f));
    GetTransform()->SetScale(Vector2(0.2f, 0.2f));

    m_speed = 0.1f;
}

void Sun::Update()
{
    GetTransform()->Rotate(m_speed);
}
