#include "../D2DEngineLib/framework.h"
#include "Sun.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Random.h"

#include "Earth.h"

void Sun::Start()
{
    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();
    
    textRenderer->SetText(L"еб╬Г");
    textRenderer->SetSpaceType(TextRenderer::SpaceType::World);
    textRenderer->SetFontSize(20.0f);
    textRenderer->SetRectSize({ 60.0f, 60.0f });
    textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
    textRenderer->SetSortOrder(2);
    textRenderer->SetVerticalAlignment(TextRenderer::VerticalAlignment::Center);
    textRenderer->SetHorizontalAlignment(TextRenderer::HorizontlAlignment::Center);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>();

    bitmapRenderer->SetBitmap(ResourceManager::Get().GetBitmap(L"SolarSystem", L"Sun"));

    GetTransform()->SetPosition(Vector2(0.0f, 0.0f));

    PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

    auto createEarth = PlayerInput::MakeAction(&Sun::CreateEarth, this);
    auto deleteEarth = PlayerInput::MakeAction(&Sun::DeleteEarth, this);

    playerInput->RegisterActionOnKey('A', PlayerInput::InputCheckType::Down, createEarth);
    playerInput->RegisterActionOnKey('S', PlayerInput::InputCheckType::Down, deleteEarth);

    m_speed = 0.2f;
}

void Sun::Update()
{
    GetTransform()->Rotate(m_speed);
}

void Sun::CreateEarth()
{
    GameObject* newEarth = CreateGameObject(L"Earth");
    newEarth->AddComponent<Earth>();
    newEarth->GetTransform()->SetPosition(Random::Float(-300.0f, 300.0f), Random::Float(-200.0f, 200.0f));

    m_earths.push_back(newEarth);
}

void Sun::DeleteEarth()
{
    if (!m_earths.empty())
    {
        m_earths.front()->Destroy();

        m_earths.pop_front();
    }
}
