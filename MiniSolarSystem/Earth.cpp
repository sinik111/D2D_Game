#include "../D2DEngineLib/framework.h"
#include "Earth.h"

#include <sstream>
#include <iomanip>

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

#include "Health.h"

void Earth::Start()
{
    auto bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
    if (bitmapRenderer == nullptr)
    {
        bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>(L"Earth.png");
    }

    bitmapRenderer->SetColor({ 0.7f, 0.5f, 0.5f, 0.5f });

    GetTransform()->SetLocalScale({ 0.5f, 0.5f });
    
    m_health = GetGameObject()->GetComponent<Health>();

    if (IsValid(m_health))
    {
        m_health->SetHp(100, 100);

        //TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

        //textRenderer->SetText(L"지구");
        //textRenderer->SetSpaceType(SpaceType::World);
        //textRenderer->SetFontSize(30.0f);
        //textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
        //textRenderer->SetSortOrder(2);
        //textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
        //textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
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
            << L"지구: WorldPosition: " << worldX << ',' << worldY
            << L"\tLocalRotation: "
            << GetTransform()->GetLocalRotation();

        // [Tip] UI 구현 시 필요한 Delegate를 통한 함수 호출
        // 각도가 바뀌었으니 등록된 함수를 호출하면서 각도에 대한 정보를 인자로 넣어줌.
        // Delegate 내부에서 유효성 검증을 하긴 하지만
        // 호출할 instance가 유효하지 않을 상황 자체를 안 만드는 게 좋을 것 같음.

        m_onRotationChange.Invoke(ss.str());

        GetTransform()->Rotate(m_speed * MyTime::DeltaTime());
    }

    if (m_isFired)
    {
        GetTransform()->Translate(Vector2::Right * 200.0f * MyTime::DeltaTime());
        GetTransform()->Rotate(-600.0f * MyTime::DeltaTime());

        //m_destroyTimer += MyTime::DeltaTime();
        //if (m_destroyTimer > 1.0f)
        //{
        //    Destroy(GetGameObject());
        //}
    }
}

void Earth::OnDestroy()
{
    if (IsValid(m_health))
    {
        m_onRotationChange.Invoke(L"지구: 파괴됨");

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
