#include "../D2DEngineLib/framework.h"
#include "Earth.h"

#include "../D2DEngineLib/GameObject.h"
#include "SpaceObject.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

void Earth::Start()
{
    TextRenderer* textRenderer = m_owner->AddComponent<TextRenderer>();

    textRenderer->SetText(L"Áö±¸");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(150.0f);
    textRenderer->SetRectSize({ 300.0f, 300.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = m_owner->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::GetBitmap(L"Earth"));

    GetTransform()->SetPosition(Vector2(800.0f, 0.0f));
    GetTransform()->SetScale(Vector2(1.0f, 1.0f));

    m_speed = 0.4f;
}

void Earth::Update()
{
    GetTransform()->Rotate(m_speed);
}
