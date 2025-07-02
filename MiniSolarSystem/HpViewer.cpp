#include "../D2DEngineLib/framework.h"
#include "HpViewer.h"

#include <sstream>

#include "../D2DEngineLib/TextRenderer.h"

void HpViewer::Initialize()
{
    m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();
}

void HpViewer::Start()
{
    m_textRenderer->SetSpaceType(SpaceType::Screen);
    m_textRenderer->SetFontSize(20.0f);
    m_textRenderer->SetPivot({ 0.0f, 0.0f });
    m_textRenderer->SetRectSize({ 200.0f, 50.0f });
    m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    m_textRenderer->SetSortOrder(3);
    m_textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
    m_textRenderer->SetHorizontalAlignment(HorizontlAlignment::Left);
}

void HpViewer::SetTargetName(const std::wstring& targetName)
{
    m_targetName = targetName;
}

void HpViewer::ChangeHpText(int prevHp, int currHp, int maxHp)
{
    std::wstringstream ss;

    ss << m_targetName << L" HP: " << currHp << L" / " << maxHp;

    m_textRenderer->SetText(ss.str());
}