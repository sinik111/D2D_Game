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

    m_textRenderer->SetSpaceType(SpaceType::Screen);
    m_textRenderer->SetFontSize(15.0f);
    m_textRenderer->SetPivot({ 0.0f, 0.0f });
    m_textRenderer->SetRectSize({ 500.0f, 400.0f });
    m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    m_textRenderer->SetSortOrder(3);
    m_textRenderer->SetVerticalAlignment(VerticalAlignment::Bottom);
    m_textRenderer->SetHorizontalAlignment(HorizontalAlignment::Left);

    GetTransform()->SetLocalPosition({ -380.0f, -280.0f });
}

void InfoViewer::LateUpdate()
{
    Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetLocalPosition();
    float cameraZoom = Camera::s_mainCamera->GetZoom();

    float sunRotation = 0.0f;
    size_t earthCount = 0;
    if (IsValid(m_sun))
    {
        sunRotation = m_sun->GetTransform()->GetLocalRotation();

        Sun* sun = m_sun->GetComponent<Sun>();
        if (IsValid(sun))
        {
            earthCount = sun->GetEarthCount();
        }
    }

    std::wostringstream oss;
    oss << std::fixed << std::setprecision(2)
        << L"FPS: " << Debug::GetLastFPS()
        << L"\nVRAM: " << FormatBytes(Debug::GetVRAMCurrentUsage())
        << L"\nDRAM: " << FormatBytes(Debug::GetDRAMCurrentUsage())
        << L"\nPageFile: " << FormatBytes(Debug::GetPageFileCurrentUsage())
        << L"\nIDXGIDevice3::Trim() [ P ]"
        << L"\nScene을 변경하려면 '3'을 누르세요"
        << L"\n지구/달에 피해 10 주기 [ T / Y ]"
        << L"\n지구 생성/삭제하기 [ A / S ] 지구 개수: " << earthCount
        << L"\n태양 Component 삭제하기 [ D ] - 회전 멈춤"
        << L"\n카메라 Position [ ← ↑ ↓ → ]: " << cameraPosition.x << L", " << cameraPosition.y
        << L"\n카메라 Zoom [ CTRL + Q / CTRL + W ]: " << cameraZoom
        << L"\n태양 Rotation: " << sunRotation
        << L"\n" << m_earthRotation
        << L"\n" << m_moonRotation;

    m_textRenderer->SetText(oss.str());
}

void InfoViewer::SetSpaceObjects(GameObject* sun)
{
	m_sun = sun;
}

void InfoViewer::SetEarthInfo(const std::wstring& earthInfo)
{
    m_earthRotation = earthInfo;
}

void InfoViewer::SetMoonInfo(const std::wstring& moonInfo)
{
    m_moonRotation = moonInfo;
}

std::wstring InfoViewer::FormatBytes(UINT64 bytes)
{
    constexpr double KB = 1024.0;
    constexpr double MB = KB * 1024.0;
    constexpr double GB = MB * 1024.0;

    std::wostringstream oss;
    oss << std::fixed << std::setprecision(2);

    if (bytes >= static_cast<UINT64>(GB))
        oss << (bytes / GB) << " GB";
    else if (bytes >= static_cast<UINT64>(MB))
        oss << (bytes / MB) << " MB";
    else if (bytes >= static_cast<UINT64>(KB))
        oss << (bytes / KB) << " KB";
    else
        oss << bytes << " B";

    return oss.str();
}
