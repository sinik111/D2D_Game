#include "../D2DEngineLib/framework.h"
#include "Moon.h"

#include <sstream>
#include <iomanip>

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Input.h"
#include "../D2DEngineLib/MyTime.h"

#include "Health.h"

Moon::~Moon()
{
    m_onRotationChange.Invoke(L"ÆÄ±«µÊ");

    GameObject* go = GameObject::Find(L"MoonHpViewer");
    if (GameObject::IsValid(go))
    {
        go->Destroy();
    }
}

void Moon::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    textRenderer->SetText(L"´Þ");
    textRenderer->SetSpaceType(SpaceType::World);
    textRenderer->SetFontSize(60.0f);
    textRenderer->SetRectSize({ 60.0f, 60.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(HorizontlAlignment::Center);

    GetGameObject()->AddComponent<BitmapRenderer>(L"Moon.png");

    GetTransform()->SetPosition({ 100.0f, 0.0f });

    m_health = GetGameObject()->GetComponent<Health>();

    m_health->SetHp(100, 100);

    m_speed = 35.0f;
}

void Moon::Update()
{
    if (Input::IsKeyPressed('Y'))
    {
        m_health->TakeDamage(10);
    }

    GetTransform()->Rotate(m_speed * MyTime::DeltaTime());

    std::wstringstream ss;
    ss << std::fixed << std::setprecision(2) << GetTransform()->GetRotation();

    m_onRotationChange.Invoke(ss.str());
}

Delegate<const std::wstring&>& Moon::GetOnRotationChange()
{
    return m_onRotationChange;
}
