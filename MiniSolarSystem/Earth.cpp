#include "../D2DEngineLib/framework.h"
#include "Earth.h"

#include <sstream>
#include <iomanip>

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Input.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/Debug.h"

#include "Health.h"

Earth::~Earth()
{
    m_onRotationChange.Invoke(L"ÆÄ±«µÊ");

    GameObject* go = GameObject::Find(L"EarthHpViewer");
    if (GameObject::IsValid(go))
    {
        go->Destroy();
    }
}

void Earth::Start()
{
    //TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    //textRenderer->SetText(L"Áö±¸");
    //textRenderer->SetSpaceType(SpaceType::World);
    //textRenderer->SetFontSize(30.0f);
    //textRenderer->SetRectSize({ 100.0f, 100.0f });
    //textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
    //textRenderer->SetSortOrder(2);
    //textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
    //textRenderer->SetHorizontalAlignment(HorizontlAlignment::Center);

    GetGameObject()->AddComponent<BitmapRenderer>(L"Earth.png");

    GetTransform()->SetLocalScale({ 0.5f, 0.5f });
    
    m_health = GetGameObject()->GetComponent<Health>();

    if (Component::IsValid(m_health))
    {
        m_health->SetHp(100, 100);
    }

    m_speed = 60.0f;
}

void Earth::Update()
{
    if (Component::IsValid(m_health))
    {
        if (Input::IsKeyPressed('T'))
        {
            m_health->TakeDamage(10);
        }
    }

    GetTransform()->Rotate(m_speed * MyTime::DeltaTime());

    float worldX = GetTransform()->GetWorldPosition().GetX();
    float worldY = GetTransform()->GetWorldPosition().GetY();

    std::wstringstream ss;
    ss << std::fixed << std::setprecision(2)
        << L"Áö±¸ WorldPosition: " << worldX << ',' << worldY
        << L"\tLocalRotation: "
        << GetTransform()->GetLocalRotation();

    m_onRotationChange.Invoke(ss.str());
}

Delegate<const std::wstring&>& Earth::GetOnRotationChange()
{
    return m_onRotationChange;
}
