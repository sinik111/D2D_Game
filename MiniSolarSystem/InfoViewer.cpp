#include "../D2DEngineLib/framework.h"
#include "InfoViewer.h"

#include <sstream>
#include <iomanip>

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "Sun.h"

void InfoViewer::Start()
{
    m_textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    m_textRenderer->SetSpaceType(TextRenderer::SpaceType::Screen);
    m_textRenderer->SetFontSize(15.0f);
    m_textRenderer->SetPoint({ -380.0f, -130.0f });
    m_textRenderer->SetRectSize({ 400.0f, 400.0f });
    m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    m_textRenderer->SetSortOrder(3);
    m_textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Top);
    m_textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Left);
}

void InfoViewer::LateUpdate()
{
    Vector2 cameraPosition{ Camera::s_mainCamera->GetTransform()->GetPosition() };
    float cameraZoom{ Camera::s_mainCamera->GetZoom() };

    float sunRotation{ 0.0f };
    size_t earthCount{ 0 };
    if (GameObject::IsValid(m_sun))
    {
        sunRotation = m_sun->GetTransform()->GetRotation();

        Sun* sun{ m_sun->GetComponent<Sun>() };
        earthCount = sun->GetEarthCount();
    }

    std::wstringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << L"Scene�� �����Ϸ��� '1'�� ��������\n"
        << L"����/�޿� ���� 10 �ֱ� [ T / Y ]\n"
        << L"���� ����/�����ϱ� [ A / S ] ���� ����: " << earthCount
        << L"\nī�޶� Position [ �� �� �� �� ]: " << cameraPosition.GetX() << L", " << cameraPosition.GetY()
        << L"\nī�޶� Zoom [ CTRL + Q / CTRL + W ]: " << cameraZoom
        << L"\n�¾� Rotation: " << sunRotation
        << L"\n���� Rotation: " << m_earthRotation
        << L"\n�� Rotation: " << m_moonRotation;

    m_textRenderer->SetText(ss.str());
}

void InfoViewer::SetSpaceObjects(GameObject* sun)
{
	m_sun = sun;
}

void InfoViewer::SetEarthRotation(const std::wstring& rotationString)
{
    m_earthRotation = rotationString;
}

void InfoViewer::SetMoonRotation(const std::wstring& rotationString)
{
    m_moonRotation = rotationString;
}