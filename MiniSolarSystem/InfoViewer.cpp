#include "../D2DEngineLib/framework.h"
#include "InfoViewer.h"

#include "../D2DEngineLib/TextRenderer.h"

void InfoViewer::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    std::wstring text = L"태양 Rotation: 0\n지구 Rotation: 0\n달 Rotation: 0";

    textRenderer->SetText(text);
    textRenderer->SetSpaceType(TextRenderer::SpaceType::Screen);
    textRenderer->SetFontSize(20.0f);
    textRenderer->SetPoint({ -380.0f, -200.0f });
    textRenderer->SetRectSize({ 300.0f, 400.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Top);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Left);
}

void InfoViewer::LateUpdate()
{
    float sunRotation = m_spaceObjects[0]->GetTransform()->GetRotation();
    float earthRotation = m_spaceObjects[1]->GetTransform()->GetRotation();
    float moonRotation = m_spaceObjects[2]->GetTransform()->GetRotation();

    std::wstring text = L"태양 Rotation: " + std::to_wstring(sunRotation) +
        L"\n지구 Rotation: " + std::to_wstring(earthRotation) + 
        L"\n달 Rotation: " + std::to_wstring(moonRotation);

    TextRenderer* textRenderer = GetGameObject()->GetComponent<TextRenderer>();
    textRenderer->SetText(text);
}

void InfoViewer::SetSpaceObjects(std::vector<GameObject*> spaceObjects)
{
	m_spaceObjects = spaceObjects;
}