#include "../D2DEngineLib/framework.h"
#include "Earth.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Input.h"
#include "../D2DEngineLib/MyTime.h"

#include "Health.h"

Earth::~Earth()
{
    m_onRotationChange.Invoke(0.0f);

    GameObject* go = GameObject::Find(L"EarthHpViewer");
    if (go != nullptr)
    {
        go->Destroy();
    }
}

void Earth::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    textRenderer->SetText(L"Áö±¸");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(30.0f);
    textRenderer->SetRectSize({ 100.0f, 100.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Earth"));

    GetTransform()->SetScale(Vector2(0.5f, 0.5f));
    
    m_health = GetGameObject()->GetComponent<Health>();

    if (m_health != nullptr)
    {
        m_health->SetHp(100, 100);
    }

    m_speed = 60.0f;
}

void Earth::Update()
{
    if (m_health != nullptr)
    {
        if (Input::IsKeyPressed('T'))
        {
            m_health->TakeDamage(10);
        }
    }

    GetTransform()->Rotate(m_speed * MyTime::DeltaTime());

    m_onRotationChange.Invoke(GetTransform()->GetRotation());
}

Delegate<float>& Earth::GetOnRotationChange()
{
    return m_onRotationChange;
}
