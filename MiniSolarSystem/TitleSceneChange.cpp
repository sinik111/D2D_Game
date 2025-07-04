#include "../D2DEngineLib/framework.h"
#include "TitleSceneChange.h"

#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/TextRenderer.h"

void TitleSceneChange::Start()
{
    PlayerInput* playerInput = GetGameObject()->AddComponent<PlayerInput>();

    playerInput->RegisterActionOnKey('2', KeyState::Pressed, this, &TitleSceneChange::ChangeScene);

    BitmapRenderer* bitmapRenderer = GetGameObject()->AddComponent<BitmapRenderer>(L"MainTitle.png");
    bitmapRenderer->SetFlipX(true);
    bitmapRenderer->SetSpaceType(SpaceType::Screen);
    

    TextRenderer* textRenderer = GetGameObject()->AddComponent<TextRenderer>();
    textRenderer->SetText(L"Scene을 변경하려면 '2'를 누르세요");
    textRenderer->SetRectSize({ 400.0f, 20.0f });
    textRenderer->SetFontSize(20.0f);
    textRenderer->SetHorizontalAlignment(HorizontlAlignment::Center);
    textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
    textRenderer->SetSpaceType(SpaceType::Screen);
    textRenderer->SetSortOrder(1);
}

void TitleSceneChange::ChangeScene()
{
    SceneManager::Get().ChangeScene(L"SolarSystem");
}
