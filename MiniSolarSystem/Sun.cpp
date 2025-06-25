#include "../D2DEngineLib/framework.h"
#include "Sun.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Random.h"
#include "../D2DEngineLib/MyTime.h"

#include "Earth.h"

void Sun::Start()
{
    TextRenderer* textRenderer{ GetGameObject()->AddComponent<TextRenderer>() };
    
    textRenderer->SetText(L"еб╬Г");
    textRenderer->SetSpaceType(SpaceType::World);
    textRenderer->SetFontSize(20.0f);
    textRenderer->SetRectSize({ 60.0f, 60.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer{ GetGameObject()->AddComponent<BitmapRenderer>() };

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Sun"));
    bitmapRenderer->SetOpacity(0.5f);
    bitmapRenderer->SetSourceRect({ 30.0f, 30.0f, 60.0f, 60.0f });

    GetTransform()->SetPosition({ 0.0f, 0.0f });

    PlayerInput* playerInput{ GetGameObject()->AddComponent<PlayerInput>() };

    playerInput->RegisterActionOnKey('A', PlayerInput::InputCheckType::Held, this, &Sun::CreateEarth);
    playerInput->RegisterActionOnKey('S', PlayerInput::InputCheckType::Held, this, &Sun::DeleteEarth);

    m_speed = 45.0f;
}

void Sun::Update()
{
    GetTransform()->Rotate(m_speed * MyTime::DeltaTime());
}

void Sun::CreateEarth()
{
    for (int i{ 0 }; i < 100; ++i)
    {
        GameObject* newEarth{ CreateGameObject(L"Earth") };
        BitmapRenderer* bitmapRenderer{ newEarth->AddComponent<BitmapRenderer>() };
        bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Earth"));
        newEarth->GetTransform()->SetPosition(Random::Float(-1600.0f, 1600.0f), Random::Float(-1200.0f, 1200.0f));

        m_earths.push_back(newEarth);
    }
}

void Sun::DeleteEarth()
{
    if (!m_earths.empty())
    {
        for (int i{ 0 }; i < 100; ++i)
        {
            m_earths.front()->Destroy();

            m_earths.pop_front();
        }
    }
}

size_t Sun::GetEarthCount() const
{
    return m_earths.size();
}