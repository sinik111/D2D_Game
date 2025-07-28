#include "../D2DEngineLib/framework.h"
#include "Title.h"

#include "../D2DEngineLib/TextRenderer.h"

void Title::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    textRenderer->SetText(L"еб╬Г╟Х");
    textRenderer->SetSpaceType(SpaceType::Screen);
    textRenderer->SetFontSize(50.0f);
    textRenderer->SetPivot({ 0.0f, 1.0f });
    textRenderer->SetRectSize({ 200.0f, 100.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
    textRenderer->SetSortOrder(3);
    textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
    textRenderer->SetHorizontalAlignment(HorizontalAlignment::Left);

    GetTransform()->SetLocalPosition({ -400.0f, 300.0f });
}