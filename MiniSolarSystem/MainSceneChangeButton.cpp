#include "../D2DEngineLib/framework.h"
#include "MainSceneChangeButton.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Button.h"

void MainSceneChangeButton::Start()
{
	auto bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	bitmapRenderer->SetSpaceType(SpaceType::Screen);
	bitmapRenderer->SetSortOrder(5);

	auto button = GetGameObject()->GetComponent<Button>();
	button->AddAction(this, &MainSceneChangeButton::ChangeScene);
}

void MainSceneChangeButton::ChangeScene()
{
	SceneManager::Get().ChangeScene(L"SolarSystem");
}
