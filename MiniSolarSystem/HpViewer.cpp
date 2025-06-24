#include "../D2DEngineLib/framework.h"
#include "HpViewer.h"

#include <sstream>

#include "../D2DEngineLib/TextRenderer.h"

void HpViewer::Start()
{
    m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();

    m_textRenderer->SetSpaceType(TextRenderer::SpaceType::Screen);
    m_textRenderer->SetFontSize(20.0f);
    m_textRenderer->SetRectSize({ 200.0f, 50.0f });
    m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    m_textRenderer->SetSortOrder(3);
    m_textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Top);
    m_textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Left);
}

void HpViewer::SetTargetName(const std::wstring& targetName)
{
    m_targetName = targetName;
}

void HpViewer::ChangeHpText(int prevHp, int currHp, int maxHp)
{
    if (m_textRenderer == nullptr)
    {
        m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();
    }

    std::wstringstream ss;

    ss << m_targetName << L" HP: " << currHp << L" / " << maxHp;

    m_textRenderer->SetText(ss.str());
}