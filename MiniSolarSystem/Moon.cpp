#include "../D2DEngineLib/framework.h"
#include "Moon.h"

#include "../D2DEngineLib/GameObject.h"
#include "SpaceObject.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

void Moon::Start()
{
    TextRenderer* textRenderer = m_owner->AddComponent<TextRenderer>();

    textRenderer->SetText(L"До");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(150.0f);
    textRenderer->SetRectSize({ 300.0f, 300.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = m_owner->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::GetBitmap(L"Moon"));

    GetTransform()->SetPosition(Vector2(400.0f, 0.0f));
    GetTransform()->SetScale(Vector2(0.5f, 0.5f));

    m_speed = 0.6f;
}

void Moon::Update()
{
    GetTransform()->Rotate(m_speed);
}
