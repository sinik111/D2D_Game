#include "../D2DEngineLib/framework.h"
#include "Moon.h"

#include <sstream>
#include <iomanip>

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

#include "Health.h"

void Moon::Start()
{
    //TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    //textRenderer->SetText(L"��");
    //textRenderer->SetSpaceType(SpaceType::World);
    //textRenderer->SetFontSize(60.0f);
    //textRenderer->SetRectSize({ 60.0f, 60.0f });
    //textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    //textRenderer->SetSortOrder(2);
    //textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
    //textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);

    GetGameObject()->AddComponent<BitmapRenderer>(L"Moon.png");

    GetTransform()->SetLocalPosition({ 200.0f, 0.0f });

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

    float worldX = GetTransform()->GetWorldPosition().x;
    float worldY = GetTransform()->GetWorldPosition().y;

    std::wstringstream ss;
    ss << std::fixed << std::setprecision(2)
        << L"��: WorldPosition: " << worldX << ',' << worldY
        << L"\tLocalRotation: "
        << GetTransform()->GetLocalRotation();

    m_onRotationChange.Invoke(ss.str());
}

void Moon::OnDestroy()
{
    m_onRotationChange.Invoke(L"��: �ı���");

    GameObject* go = GameObject::Find(L"MoonHpViewer");
    if (IsValid(go))
    {
        Destroy(go);
    }
}

Delegate<const std::wstring&>& Moon::GetOnRotationChange()
{
    return m_onRotationChange;
}
