#include "../D2DEngineLib/framework.h"
#include "InfoViewer.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Camera.h"

void InfoViewer::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    std::wstring text = L"카메라 Position: 0, 0\n태양 Rotation: 0\n지구 Rotation: 0\n달 Rotation: 0";

    textRenderer->SetText(text);
    textRenderer->SetSpaceType(TextRenderer::SpaceType::Screen);
    textRenderer->SetFontSize(15.0f);
    textRenderer->SetPoint({ -380.0f, -200.0f });
    textRenderer->SetRectSize({ 400.0f, 400.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    textRenderer->SetSortOrder(3);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Top);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Left);
}

void InfoViewer::LateUpdate()
{
    Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetPosition();

    float sunRotation = m_spaceObjects[0]->GetTransform()->GetRotation();
    float earthRotation = m_spaceObjects[1]->GetTransform()->GetRotation();
    float moonRotation = m_spaceObjects[2]->GetTransform()->GetRotation();

    std::wstring text = L"카메라 Position: " + std::to_wstring(cameraPosition.GetX()) + L", " 
        + std::to_wstring(cameraPosition.GetY())
        + L"\n태양 Rotation: " + std::to_wstring(sunRotation) +
        L"\n지구 Rotation: " + std::to_wstring(earthRotation) + 
        L"\n달 Rotation: " + std::to_wstring(moonRotation);

    TextRenderer* textRenderer = GetGameObject()->GetComponent<TextRenderer>();
    textRenderer->SetText(text);
}

void InfoViewer::SetSpaceObjects(std::vector<GameObject*> spaceObjects)
{
	m_spaceObjects = spaceObjects;
}