#include "../D2DEngineLib/framework.h"
#include "Sun.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/BatchRenderer.h"

#include "Earth.h"

void Sun::Start()
{
    //TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();
    //
    //textRenderer->SetText(L"еб╬Г");
    //textRenderer->SetSpaceType(SpaceType::World);
    //textRenderer->SetFontSize(20.0f);
    //textRenderer->SetRectSize({ 60.0f, 60.0f });
    //textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
    //textRenderer->SetSortOrder(2);
    //textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
    //textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>(L"Sun.png");
    //bitmapRenderer->SetOpacity(0.3f);
    //bitmapRenderer->SetSourceRect({ 30.0f, 30.0f, 60.0f, 60.0f });
    //bitmapRenderer->SetPivot({ 0.5f, 0.0f });

    PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

    //playerInput->RegisterActionOnKey('A', KeyState::Held, this, &Sun::CreateEarth);
    //playerInput->RegisterActionOnKey('S', KeyState::Held, this, &Sun::DeleteEarth);
    playerInput->RegisterActionOnKey('D', KeyState::Pressed, this, &Sun::DestroySelf);

    m_speed = 45.0f;

    m_batchTest = CreateGameObject(L"BatchTest");
    auto batchRenderer = m_batchTest->AddComponent<BatchRenderer>(L"ken.png", L"ken_sprites.json");
    batchRenderer->SetLocalRect({ -960.0f, -540.0f, 960.0f, 540.0f });
    batchRenderer->SetSortOrder(3);
}

void Sun::Update()
{
    GetTransform()->Rotate(m_speed * MyTime::DeltaTime());

    if (Input::IsKeyHeld('A'))
    {
        auto batchRenderer = m_batchTest->GetComponent<BatchRenderer>();

        for (int i = 0; i < 100; ++i)
        {
            BatchUnit unit;
            unit.index = Random::Int(0, 470);
            unit.position = { Random::Float(-1920.0f, 1920.0f) * 4, Random::Float(-1080.0f, 1080.0f) * 4 };
            unit.color = {
                Random::Float(0.0f, 1.0f),
                Random::Float(0.0f, 1.0f),
                Random::Float(0.0f, 1.0f),
                Random::Float(0.0f, 1.0f)
            };

            batchRenderer->AddBatchUnit(unit);
        }
    }

    if (Input::IsKeyPressed('S'))
    {
        m_batchTest->GetComponent<BatchRenderer>()->ClearBatchUnitList();
    }
}

void Sun::OnDestroy()
{
    PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();

    playerInput->UnregisterActionOnKey('A', this);
    playerInput->UnregisterActionOnKey('S', this);
    playerInput->UnregisterActionOnKey('D', this);
}

void Sun::OnTriggerEnter(const Collision& collision)
{
    if (collision.otherGameObject->GetName() == L"Ken")
    {
        Destroy(GetGameObject());
    }
}

void Sun::CreateEarth()
{
    for (int i = 0; i < 100; ++i)
    {
        GameObject* newEarth = CreateGameObject(L"Earth");
        newEarth->AddComponent<Earth>();
        auto bitmap = newEarth->AddComponent<BitmapRenderer>(L"Earth.png");
        bitmap->SetSortOrder(Random::Int(0, 29));
        newEarth->GetTransform()->SetLocalPosition(Random::Float(-1600.0f, 1600.0f), Random::Float(-1200.0f, 1200.0f));

        m_earths.push_back(newEarth);
    }
}

void Sun::DeleteEarth()
{
    for (int i = 0; i < 100; ++i)
    {
        if (!m_earths.empty())
        {
            Destroy(m_earths.front());

            m_earths.pop_front();
        }
    }
}

void Sun::DestroySelf()
{
    Destroy(this);
}

size_t Sun::GetEarthCount() const
{
    if (m_batchTest != nullptr)
    {
        return m_batchTest->GetComponent<BatchRenderer>()->GetBatchCount();
    }
    else
    {
        return 0;
    }
}