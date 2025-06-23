#include "../D2DEngineLib/framework.h"
#include "Moon.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Input.h"
#include "Health.h"

Moon::~Moon()
{
    m_onRotationChange.Invoke(0.0f);

    GameObject* go = GameObject::Find(L"MoonHpViewer");
    if (go != nullptr)
    {
        go->Destroy();
    }
}

void Moon::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();

    textRenderer->SetText(L"До");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(60.0f);
    textRenderer->SetRectSize({ 60.0f, 60.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Moon"));

    GetTransform()->SetPosition(Vector2(100.0f, 0.0f));

    m_health = GetGameObject()->GetComponent<Health>();

    m_health->SetHp(100, 100);

    m_speed = 0.5f;
}

void Moon::Update()
{
    if (Input::IsKeyPressed('Y'))
    {
        m_health->TakeDamage(10);
    }

    GetTransform()->Rotate(m_speed);

    m_onRotationChange.Invoke(GetTransform()->GetRotation());
}

Delegate<float>& Moon::GetOnRotationChange()
{
    return m_onRotationChange;
}
