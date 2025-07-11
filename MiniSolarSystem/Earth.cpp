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

void Earth::Start()
{
    auto bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
    if (bitmapRenderer == nullptr)
    {
        GetGameObject()->AddComponent<BitmapRenderer>(L"Earth.png");
    }

    GetTransform()->SetLocalScale({ 0.5f, 0.5f });
    
    m_health = GetGameObject()->GetComponent<Health>();

    if (IsValid(m_health))
    {
        m_health->SetHp(100, 100);

        //TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

        //textRenderer->SetText(L"Áö±¸");
        //textRenderer->SetSpaceType(SpaceType::World);
        //textRenderer->SetFontSize(30.0f);
        //textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
        //textRenderer->SetSortOrder(2);
        //textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
        //textRenderer->SetHorizontalAlignment(HorizontlAlignment::Center);
        //textRenderer->SetRectSize({ 100.0f, 100.0f });
    }

    m_speed = 60.0f;
}

void Earth::Update()
{
    if (IsValid(m_health))
    {
        if (Input::IsKeyPressed('T'))
        {
            m_health->TakeDamage(10);
        }
        float worldX = GetTransform()->GetWorldPosition().x;
        float worldY = GetTransform()->GetWorldPosition().y;

        std::wstringstream ss;
        ss << std::fixed << std::setprecision(2)
            << L"Áö±¸: WorldPosition: " << worldX << ',' << worldY
            << L"\tLocalRotation: "
            << GetTransform()->GetLocalRotation();

        m_onRotationChange.Invoke(ss.str());

        GetTransform()->Rotate(m_speed * MyTime::GetDeltaTime());
    }

    if (m_isFired)
    {
        GetTransform()->Translate(Vector2::Right * 200.0f * MyTime::GetDeltaTime());
        GetTransform()->Rotate(-600.0f * MyTime::GetDeltaTime());

        m_destroyTimer += MyTime::GetDeltaTime();
        if (m_destroyTimer > 1.0f)
        {
            Destroy(GetGameObject());
        }
    }
}

void Earth::OnDestroy()
{
    if (IsValid(m_health))
    {
        m_onRotationChange.Invoke(L"Áö±¸: ÆÄ±«µÊ");

        GameObject* go = GameObject::Find(L"EarthHpViewer");
        if (IsValid(go))
        {
            Destroy(go);
        }
    }
}

Delegate<const std::wstring&>& Earth::GetOnRotationChange()
{
    return m_onRotationChange;
}

void Earth::Fired()
{
    m_isFired = true;
}
